using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using OCR.TesseractWrapper;
using System.Drawing;
using IPoVn.IPCore;
using System.Drawing.Imaging;

namespace IPoVn.OCRer
{
    internal class Simple4
    {
        public static void Recognize()
        {
            int n_images = Workspace.Images.Length;
            int i_image = n_images - 1;
            i_image = 0;
            string fileName = Workspace.Images[i_image];

            string imageFile = Path.Combine(Workspace.InputFolder, fileName);

            string language = "eng";
            int oem = 3;

            TesseractProcessor processor = new TesseractProcessor();

            using (Bitmap bmp = Bitmap.FromFile(imageFile) as Bitmap)
            {
                GreyImage greyImage = GreyImage.FromImage(bmp);

                DateTime started = DateTime.Now;
                AdaptiveThresholder thresholder = new AdaptiveThresholder();
                BinaryImage binImage = thresholder.Threshold(greyImage);
                DateTime ended = DateTime.Now;
                Console.WriteLine("Duration thresholding: {0} ms", (ended - started).TotalMilliseconds);

                binImage.Invert();

                //for (int i = 0; i < 4; i++)
                for (int i = 3; i < 4; i++)
                {
                    oem = i;
                    oem = (int)eOcrEngineMode.OEM_TESSERACT_CUBE_COMBINED;
                    processor.Init(Workspace.TessdataFolder, language, oem);

                    string text = "";
                    unsafe
                    {
                        started = DateTime.Now;

                        //string text = processor.Recognize(bmp);
                        text = processor.RecognizeBinaryImage(
                            binImage.BinaryData, binImage.Width, binImage.Height);

                        ended = DateTime.Now;
                        Console.WriteLine("Duration recognition: {0} ms\n\n", (ended - started).TotalMilliseconds);
                    }

                    Console.WriteLine(
                        string.Format("RecognizeMode: {1}\nText:\n{0}\n++++++++++++++++++++++++++++++++\n", text, ((eOcrEngineMode)oem).ToString()));
                }
            }
        }

        public static void AnalyseLayout()
        {
            int n_images = Workspace.Images.Length;
            int i_image = n_images - 1;
            //for (; i_image < image_names.Length; i_image++)
            {
                string fileName = Workspace.Images[i_image];

                Console.WriteLine("{0} Image: {1}", i_image, fileName);

                string imageFile = Path.Combine(Workspace.InputFolder, fileName);

                string name = Path.GetFileNameWithoutExtension(imageFile);

                string outFile = Path.Combine(Workspace.OutputFolder, string.Format("{0}_layout.bmp", name));
                string outFile2 = Path.Combine(Workspace.OutputFolder, string.Format("{0}_grey.bmp", name));
                string outFile3 = Path.Combine(Workspace.OutputFolder, string.Format("{0}_bin.bmp", name));

                TesseractProcessor processor = new TesseractProcessor();
                processor.InitForAnalysePage();
                //processor.SetPageSegMode(ePageSegMode.PSM_AUTO);

                using (Bitmap bmp = Bitmap.FromFile(imageFile) as Bitmap)
                {
                    GreyImage greyImage = GreyImage.FromImage(bmp);
                    //greyImage.Save(ImageFormat.Bmp, outFile2);

                    DateTime started = DateTime.Now;

                    AdaptiveThresholder thresholder = new AdaptiveThresholder();
                    BinaryImage binImage = thresholder.Threshold(greyImage);

                    DateTime ended = DateTime.Now;
                    Console.WriteLine("Duration thresholding: {0} ms", (ended - started).TotalMilliseconds);

                    //binImage.Invert();
                    binImage.Save(ImageFormat.Bmp, outFile3);

                    DocumentLayout doc = null;


                    unsafe
                    {
                        started = DateTime.Now;
                        doc = processor.AnalyseLayoutBinaryImage(
                            binImage.BinaryData, binImage.Width, binImage.Height);
                        ended = DateTime.Now;
                        Console.WriteLine("Duration AnalyseLayout: {0} ms", (ended - started).TotalMilliseconds);
                    }
                    Console.WriteLine(doc.ToString());

                    using (Image tmp = new Bitmap(bmp.Width, bmp.Height)) // prevents one-byte index format
                    {
                        using (Graphics grph = Graphics.FromImage(tmp))
                        {
                            Rectangle rect = new Rectangle(0, 0, tmp.Width, tmp.Height);

                            grph.DrawImage(bmp, rect, rect, GraphicsUnit.Pixel);

                            grph.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;

                            foreach (Block block in doc.Blocks)
                            {
                                Render.DrawBlock(grph, block);
                            }
                        }

                        tmp.Save(outFile);
                    }
                }
            }
        }
    }
}
