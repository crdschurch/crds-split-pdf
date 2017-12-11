//
//  SearchPDF.c
//
//  Created by Ingage Partners on 7/11/17.
//  Copyright © 2017 Crossroads. All rights reserved.
//

#include <ApplicationServices/ApplicationServices.h>
#include "SearchPDF.h"

typedef struct {
    CFStringRef searchText;
    bool foundText;
} CallbackData;

static void ArrayCallback(CGPDFScannerRef scanner, void* userInfo);
static void StringCallback(CGPDFScannerRef scanner, void* userInfo);


// Search all pages in the PDF document and return a list of 0-based page indices that contain the
// searchText.
bool FindPagesWithText(CGPDFDocumentRef document, CFStringRef searchText, std::vector<size_t>& pageIndex)
{
    CGPDFOperatorTableRef table = CGPDFOperatorTableCreate();
    CGPDFOperatorTableSetCallback(table, "TJ", ArrayCallback);
    CGPDFOperatorTableSetCallback(table, "Tj", StringCallback);

    size_t numPages = CGPDFDocumentGetNumberOfPages(document);
    for (size_t i = 0; i < numPages; i++)
    {
        CallbackData callbackData;
        callbackData.searchText = searchText;
        callbackData.foundText = false;
        
        CGPDFPageRef page = CGPDFDocumentGetPage(document, i + 1);
        if (page != NULL)
        {
            CGPDFContentStreamRef contentStream = CGPDFContentStreamCreateWithPage(page);
            CGPDFScannerRef scanner = CGPDFScannerCreate(contentStream, table, &callbackData);
            bool ret = CGPDFScannerScan(scanner);
            CGPDFScannerRelease(scanner);
            CGPDFContentStreamRelease(contentStream);
            
            if (callbackData.foundText) {
                pageIndex.push_back(i);
            }
        }
    }

    CGPDFOperatorTableRelease(table);
    
    return pageIndex.size() > 0 ? true : false;
}


static void ArrayCallback(CGPDFScannerRef scanner, void* userInfo)
{
    CallbackData* callbackData = (CallbackData*)userInfo;
    
    CGPDFArrayRef array;
    bool success = CGPDFScannerPopArray(scanner, &array);
    
    if (success)
    {
        for (size_t i = 0; i < CGPDFArrayGetCount(array); i += 2)
        {
            CGPDFStringRef string;
            success = CGPDFArrayGetString(array, i, &string);
            if (success)
            {
                CFStringRef stringRef = CGPDFStringCopyTextString(string);
                if (stringRef != NULL) {
                    if (CFStringCompare(stringRef, callbackData->searchText, 0) == kCFCompareEqualTo)
                        callbackData->foundText = true;
                    
                    CFRelease(stringRef);
                }
            }
        }
    }
}


static void StringCallback(CGPDFScannerRef scanner, void* userInfo)
{
    CallbackData* callbackData = (CallbackData*)userInfo;
    
    CGPDFStringRef string;
    bool success = CGPDFScannerPopString(scanner, &string);

    if (success)
    {
        CFStringRef stringRef = CGPDFStringCopyTextString(string);
        if (stringRef != NULL) {
            if (CFStringCompare(stringRef, callbackData->searchText, 0) == kCFCompareEqualTo)
                callbackData->foundText = true;
            
            CFRelease(stringRef);
        }
    }
}
