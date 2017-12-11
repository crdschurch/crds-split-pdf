//
//  SplitPDF.h
//
//  Created by Ingage Partners on 7/11/17.
//  Copyright © 2017 Crossroads. All rights reserved.
//

#ifndef SplitPDF_h
#define SplitPDF_h

#include <ApplicationServices/ApplicationServices.h>

size_t SplitPDF(CFURLRef destinationFolderUrl, CFStringRef baseFilename, CGPDFDocumentRef document);

#endif /* SplitPDF_h */
