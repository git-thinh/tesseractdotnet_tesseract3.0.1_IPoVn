using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace IPoVn.OCRer
{
    internal class Workspace
    {
        public static string RootFolder = "";
        public static string InputFolder = "";
        public static string OutputFolder = "";
        public static string TessdataFolder = "";
        public static string[] Images = new string[] {
            @"phototest.tif",
            @"eurotext.tif",
            @"sample3.png",
            @"sample4.jpg"
        };

        public static string ControlSet1 = "";
        public static string ControlSet2 = "";
        public static string ControlSet3 = "";
        public static string ControlSet4 = "";

        public static void InitializeWorkspace()
        {
            try
            {
                Console.WriteLine("Tesseract Engine .net Wrapper @CongNguyen\n\n");

                string executedPath = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
                string[] folders = executedPath.Split('\\');
                int solution = 0;
                for (int i = folders.Length - 1; i >= 0; i--)
                {
                    if (string.Compare(folders[i].Trim(), "IPoVnOCRer", true) == 0)
                    {
                        solution = i;
                        break;
                    }
                }

                RootFolder = "";
                for (int i = 0; i <= solution; i++)
                {
                    RootFolder = string.Format("{0}{1}\\", RootFolder, folders[i]);

                    if (i == solution)
                        break;
                }

                InputFolder = Path.Combine(RootFolder, "IPoVn\\Test\\Input\\");
                OutputFolder = Path.Combine(RootFolder, "IPoVn\\Test\\Output\\");
                TessdataFolder = Path.Combine(RootFolder, "IPoVn\\Test\\tessdata\\");

                // clear output folder
                if (!Directory.Exists(OutputFolder))
                {
                    Directory.CreateDirectory(OutputFolder);
                }
                string[] files = Directory.GetFiles(OutputFolder);
                if (files != null && files.Length > 0)
                {
                    for (int i = files.Length - 1; i >= 0; i--)
                    {
                        File.Delete(files[i]);
                    }
                }
            }
            catch
            {
            }
            finally
            {
            }
        }
    }
}
