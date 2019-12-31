using System;
using System.Collections.Generic;
using System.Text;
using OCR.TesseractWrapper;
using System.Drawing;
using IPoVn.IPCore;
using System.IO;
using System.Drawing.Imaging;

namespace IPoVn.OCRer
{
    class Program
    {
        static void Main(string[] args)
        {
            Workspace.InitializeWorkspace();

            // simple1
            Simple1.Recognize();
            Simple1.AnalyseLayout();

            // simple2
            Simple2.Recognize();
            Simple2.AnalyseLayout();

            // simple3
            Simple3.Recognize();


            Console.Write("\n\n\nPress any key to exit...");
            Console.ReadKey();
        }
    }
}
