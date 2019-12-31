tesseractdotnet based on tesseract-orc v3.01 r590

- added wrappers (refer to folder: wrapper)

- added "tessedit_thresholding_method" in tesseract-ocr (refer to: wrapper\lathr.h)

- sample: refer to tesseractconsole project.

Usage threshold mode:

	- call SetVariable() method before passing image (api->SetImage(image))
	// 0: otsu
        // 1: isodata local adaptive
        // 2: sauvola local adaptive => not implement yet
        //processor.SetVariable("tessedit_thresholding_method", "0");
	processor.SetVariable("tessedit_thresholding_method", "1");



