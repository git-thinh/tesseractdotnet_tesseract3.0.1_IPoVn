using OCR.TesseractWrapper;
using System;
using System.Drawing;
using System.IO;
using System.Reflection;

namespace IPoVn.OCRer
{
    public enum OCR_ENGINE_MODE
    {
        OEM_TESSERACT_ONLY = 0,
        OEM_CUBE_ONLY = 1,
        OEM_TESSERACT_CUBE_COMBINED = 2,
        OEM_DEFAULT = 3
    }

    class Program
    {
        //const string path_tessdata = @"C:\IPoVn\Test\tessdata\";
        const string path_tessdata = @"tessdata\";

        const string OutputFolder = "_";
        static string[] Images = new string[] {
            @"phototest.tif",
            @"eurotext.tif",
            @"sample3.png",
            @"sample4.jpg"
        };

        static void Simple1_Recognize()
        {
            using (TesseractProcessor processor = new TesseractProcessor())
            {
                using (Bitmap bmp = Bitmap.FromFile("phototest.tif") as Bitmap)
                {
                    DateTime started = DateTime.Now;
                    DateTime ended = DateTime.Now;

                    int oem = 0;
                    for (int i = 0; i < 4; i++)
                    //for (int i = 3; i < 4; i++)
                    {
                        oem = i;
                        bool ok = processor.Init(path_tessdata, "eng", i); 
                        if (ok)
                        {
                            string text = "";
                            unsafe
                            {
                                started = DateTime.Now;

                                text = processor.Recognize(bmp);

                                ended = DateTime.Now;

                                Console.WriteLine("Duration recognition: {0} ms\n\n", (ended - started).TotalMilliseconds);
                            }

                            Console.WriteLine(
                                string.Format("RecognizeMode: {1}\nRecognized Text:\n{0}\n++++++++++++++++++++++++++++++++\n", text, ((eOcrEngineMode)oem).ToString()));
                        }
                        else
                        {
                            Console.WriteLine("FAIL " + i.ToString());
                        }
                    }
                }
            }
        }

        static void Simple1_AnalyseLayout()
        {
            int n_images = Images.Length;
            int i_image = 0;
            for (; i_image < n_images; i_image++)
            {
                string fileName = Images[i_image];

                Console.WriteLine("{0} Image: {1}", i_image, fileName);

                string imageFile = Path.Combine("", fileName);

                string name = Path.GetFileNameWithoutExtension(imageFile);

                string outFile = Path.Combine(OutputFolder, string.Format("Simple1_{0}_layout.bmp", name));
                string outFile2 = Path.Combine(OutputFolder, string.Format("Simple1_{0}_grey.bmp", name));
                string outFile3 = Path.Combine(OutputFolder, string.Format("Simple1_{0}_bin.bmp", name));

                using (TesseractProcessor processor = new TesseractProcessor())
                {
                    processor.InitForAnalysePage();
                    //processor.SetPageSegMode(ePageSegMode.PSM_AUTO);

                    using (Bitmap bmp = Bitmap.FromFile(imageFile) as Bitmap)
                    {
                        DateTime started = DateTime.Now;
                        DateTime ended = DateTime.Now;

                        DocumentLayout doc = null;

                        unsafe
                        {
                            started = DateTime.Now;

                            doc = processor.AnalyseLayout(bmp);

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

        static void Simple2_Recognize() { }
        static void Simple2_AnalyseLayout() { }

        static void Simple3_Recognize()
        {
        }


        static void Main(string[] args)
        {
            //Simple1_Recognize();
            Simple1_AnalyseLayout();

            //Simple2_Recognize();
            //Simple2_AnalyseLayout();

            //Simple3_Recognize();

            Console.Write("\n\n\nPress any key to exit...");
            Console.ReadKey();
        }
    }
}
