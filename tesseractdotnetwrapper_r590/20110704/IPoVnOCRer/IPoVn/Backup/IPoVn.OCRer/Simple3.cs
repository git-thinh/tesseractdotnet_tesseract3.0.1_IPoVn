#define PARALLEL

using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using OCR.TesseractWrapper;
using System.Drawing;
using IPoVn.IPCore;
using System.Drawing.Imaging;
using IPoVnSystem;
using System.Diagnostics;

namespace IPoVn.OCRer
{
    internal class Simple3
    {
        public static void Recognize()
        {
            int n_images = Workspace.Images.Length;
            int i_image = n_images - 1;
            //i_image = 0;
            i_image = 2;
            string fileName = Workspace.Images[i_image];

            string imageFile = Path.Combine(Workspace.InputFolder, fileName);

            string language = "eng";
            int oem = (int)eOcrEngineMode.OEM_DEFAULT;

            string name = Path.GetFileNameWithoutExtension(imageFile);
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

                            Rectangle[] rois = new Rectangle[] { 
                                Rectangle.FromLTRB(807, 43, 1351, 613),
                                Rectangle.FromLTRB(4, 604, binImage.Width - 15, binImage.Height-35)
                            };

                            int nROIs = rois.Length;

                            string[] texts = new string[nROIs];
#if PARALLEL
                            Parallel.For(0, nROIs, delegate(int iROI) 
#else
                            using (TesseractProcessor processor = new TesseractProcessor())
                            for (int iROI = 0; iROI < nROIs; iROI++)
#endif
                            {
#if PARALLEL
                                using (TesseractProcessor processor = new TesseractProcessor())
#endif
                                {
                                    Rectangle roi = rois[iROI];
                                    {                                    
                                        //oem = (int)eOcrEngineMode.OEM_TESSERACT_CUBE_COMBINED;
                                        processor.Init(Workspace.TessdataFolder, language, oem);
                                        processor.UseROI = true;
                                        processor.ROI = roi;
                                        unsafe
                                        {                                            
                                            texts[iROI] = processor.RecognizeBinaryImage(
                                               binImage.BinaryData, binImage.Width, binImage.Height);
                                        }
                                    }
                                }
                            }
#if PARALLEL
                            );
#endif

                            ended = DateTime.Now;

                            Console.WriteLine("Duration recognition: {0} ms\n\n", (ended - started).TotalMilliseconds);

                            Console.WriteLine("Recognized Text:");
                            for (int i = 0; i < nROIs; i++)
                            {
                                Console.WriteLine(texts[i]);
                            }

                            string txtFile = Path.Combine(
                                Workspace.OutputFolder, string.Format("Simple3_{0}.txt", name));
                            using (StreamWriter writer = new StreamWriter(txtFile))
                            {
                                for (int i = 0; i < nROIs; i++)
                                {
                                    writer.WriteLine(texts[i]);
                                    writer.WriteLine("\n\n");
                                }
                            }
                            Process.Start(txtFile);
                        }
                    }
                }
            }
        }
    }
}
