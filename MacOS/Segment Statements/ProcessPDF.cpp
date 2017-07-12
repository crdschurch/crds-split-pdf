//
//  ProcessPDF.c
//
//  Created by Ingage Partners on 7/11/17.
//  Copyright © 2017 Crossroads. All rights reserved.
//

#include "ProcessPDF.h"
#include "SplitPDF.h"


int ProcessPDF(CFURLRef inputFile, CFURLRef outputPath, CFStringRef baseName)
{
    int totalStatements = -1;
    
    CGPDFDocumentRef document = CGPDFDocumentCreateWithURL(inputFile);
    if (document != NULL) {
        totalStatements = (int) SplitPDF(outputPath, baseName, document);

        CGPDFDocumentRelease(document);
    }
    
    return totalStatements;
}
