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
    internal class Simple2
    {
        public static void Recognize()
        {
            int n_images = Workspace.Images.Length;
            int i_image = n_images - 1;
            //i_image = 0;
            string fileName = Workspace.Images[i_image];

            string imageFile = Path.Combine(Workspace.InputFolder, fileName);

            string language = "eng";
            int oem = (int)eOcrEngineMode.OEM_DEFAULT;

            using (TesseractProcessor processor = new TesseractProcessor())
            {
                using (Bitmap bmp = Bitmap.FromFile(imageFile) as Bitmap)
                {
                    using (GreyImage greyImage = GreyImage.FromImage(bmp))
                    {

                        ImageThresholder thresholder = new AdaptiveThresholder();
                        using (BinaryImage binImage = thresholder.Threshold(greyImage))
                        {
                            DateTime started = DateTime.Now;
                            DateTime ended = DateTime.Now;

                            int i = 3;
                            //for (i = 0; i < 4; i++)
                            //for (i = 3; i < 4; i++)
                            {
                                oem = i;
                                processor.Init(Workspace.TessdataFolder, language, oem);

                                string text = "";
                                unsafe
                                {
                                    started = DateTime.Now;

                                    text = processor.RecognizeBinaryImage(
                                        binImage.BinaryData, greyImage.Width, greyImage.Height);

                                    ended = DateTime.Now;

                                    Console.WriteLine("Duration recognition: {0} ms\n\n", (ended - started).TotalMilliseconds);
                                }

                                Console.WriteLine(
                                    string.Format("RecognizeMode: {1}\nRecognized Text:\n{0}\n++++++++++++++++++++++++++++++++\n", text, ((eOcrEngineMode)oem).ToString()));
                            }
                        }
                    }
                }
            }
        }

        public static void AnalyseLayout()
        {
            int n_images = Workspace.Images.Length;
            int i_image = 0;
            for (; i_image < n_images; i_image++)
            {
                string fileName = Workspace.Images[i_image];

                Console.WriteLine("{0} Image: {1}", i_image, fileName);

                string imageFile = Path.Combine(Workspace.InputFolder, fileName);

                string name = Path.GetFileNameWithoutExtension(imageFile);

                string outFile = Path.Combine(Workspace.OutputFolder, string.Format("Simple2_{0}_layout.bmp", name));
                string outFile2 = Path.Combine(Workspace.OutputFolder, string.Format("Simple2_{0}_grey.bmp", name));
                string outFile3 = Path.Combine(Workspace.OutputFolder, string.Format("Simple2_{0}_bin.bmp", name));

                using (TesseractProcessor processor = new TesseractProcessor())
                {
                    processor.InitForAnalysePage();
                    //processor.SetPageSegMode(ePageSegMode.PSM_AUTO);

                    using (Bitmap bmp = Bitmap.FromFile(imageFile) as Bitmap)
                    {
                        using (GreyImage greyImage = GreyImage.FromImage(bmp))
                        {
                            greyImage.Save(ImageFormat.Bmp, outFile2);

                            ImageThresholder thresholder = new AdaptiveThresholder();
                            using (BinaryImage binImage = thresholder.Threshold(greyImage))
                            {
                                binImage.Save(ImageFormat.Bmp, outFile3);

                                DateTime started = DateTime.Now;
                                DateTime ended = DateTime.Now;

                                DocumentLayout doc = null;

                                unsafe
                                {
                                    started = DateTime.Now;

                                    doc = processor.AnalyseLayoutBinaryImage(
                                        binImage.BinaryData, greyImage.Width, greyImage.Height);

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
        }
    }
}
