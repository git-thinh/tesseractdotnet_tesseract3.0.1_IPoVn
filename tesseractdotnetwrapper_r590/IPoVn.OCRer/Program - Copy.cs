using System;
using System.IO;
using System.Reflection;

namespace IPoVn.OCRer
{
    class Program
    {
        static Program()
        {
            AppDomain.CurrentDomain.AssemblyResolve += (se, ev) =>
            {
                Assembly asm = null;
                string comName = ev.Name.Split(',')[0];

                string resourceName = @"dll\" + comName + ".dll";
                var assembly = Assembly.GetExecutingAssembly();
                resourceName = typeof(Program).Namespace + "." + resourceName.Replace(" ", "_").Replace("\\", ".").Replace("/", ".");
                using (Stream stream = assembly.GetManifestResourceStream(resourceName))
                //using (Stream stream = File.OpenRead("dll/" + comName + ".dll"))
                {
                    if (stream == null)
                    {
                        //Debug.WriteLine(resourceName);
                    }
                    else
                    {
                        byte[] buffer = new byte[stream.Length];
                        using (MemoryStream ms = new MemoryStream())
                        {
                            int read;
                            while ((read = stream.Read(buffer, 0, buffer.Length)) > 0)
                                ms.Write(buffer, 0, read);
                            buffer = ms.ToArray();
                        }
                        asm = Assembly.Load(buffer);
                    }
                }
                return asm;
            };
        }

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
