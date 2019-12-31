using OCR.TesseractWrapper;
using System;
using System.Drawing;
using System.IO;
using System.Reflection;

namespace IPoVn.OCRer
{
    class Program
    { 
        static void Main(string[] args)
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
                        bool ok = processor.Init(@"C:\IPoVn\Test\tessdata\", "eng", i);
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
                        else {
                            Console.WriteLine("FAIL " + i.ToString());
                        }
                    }
                }
            }

            Console.Write("\n\n\nPress any key to exit...");
            Console.ReadKey();
        }
    }
}
