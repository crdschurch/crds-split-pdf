//
//  SplitPDF.c
//
//  Created by Ingage Partners on 7/11/17.
//  Copyright © 2017 Crossroads. All rights reserved.
//

#include <vector>
#include "SearchPDF.h"
#include "SplitPDF.h"

typedef struct {
    size_t startPageIndex;
    size_t pageCount;
} StatementInfo;

static bool BuildStatementList(std::vector<StatementInfo>& statementList, CGPDFDocumentRef document);
static std::vector<StatementInfo> GetSubset(const std::vector<StatementInfo>& statementList, size_t pageCountTarget, bool includeExtras);
static CFURLRef CreateFileName(CFURLRef destinationFolderUrl, CFStringRef baseFilename, CFStringRef suffix);
static int CreateFile(CFURLRef destinationUrl, CGPDFDocumentRef document, const std::vector<StatementInfo>& statementList);


size_t SplitPDF(CFURLRef destinationFolderUrl, CFStringRef baseFilename, CGPDFDocumentRef document)
{
    std::vector<StatementInfo> statementList;
    if (!BuildStatementList(statementList, document))
        return -1;
    
    size_t totalStatements = 0;

    int pageCount[] = { 1, 2, 3, 4 };
    int numEntries = sizeof(pageCount) / sizeof(pageCount[0]);
    for (int i = 0; i < numEntries; ++i) {
        bool isLast = (i == numEntries - 1) ? true : false;
        std::vector<StatementInfo> subset = GetSubset(statementList, pageCount[i], isLast);
        
        if (subset.size() > 0) {
            CFStringRef suffix = NULL;
            switch(pageCount[i]) {
                case 1:
                    suffix = CFSTR(" 1-Page");
                    break;
                case 2:
                    suffix = CFSTR(" 2-Page");
                    break;
                case 3:
                    suffix = CFSTR(" 3-Page");
                    break;
                case 4:
                    suffix = CFSTR(" 4-Page");
                    break;
            }
            
            CFURLRef fileName = CreateFileName(destinationFolderUrl, baseFilename, suffix);
            int numPages = CreateFile(fileName, document, subset);
            CFRelease(fileName);
            
            if (numPages <= 0)
                return -1;
            
            totalStatements += subset.size();
        }
    }

    return totalStatements;
}


static bool BuildStatementList(std::vector<StatementInfo>& statementList, CGPDFDocumentRef document)
{
    size_t totalPages = CGPDFDocumentGetNumberOfPages(document);
    
    std::vector<size_t> pageIndex;
    FindPagesWithText(document, CFSTR("Contribution Statement"), pageIndex);
    if (pageIndex.size() == 0)
        return false;           // couldn't find any matching text
    
    if (pageIndex[0] != 0)
        return false;           // extra page(s) at the start (did we miss something?)

    for (int i = 0; i < pageIndex.size(); ++i) {
        size_t currentIndex = pageIndex[i];
        size_t nextIndex = (i + 1 < pageIndex.size()) ? pageIndex[i + 1] : totalPages;
        
        size_t pageCount = nextIndex - currentIndex;
        if (pageCount > 0) {
            StatementInfo info;
            info.startPageIndex = currentIndex;
            info.pageCount = pageCount;
            statementList.push_back(info);
        }
    }
    
    return true;
}


static std::vector<StatementInfo> GetSubset(const std::vector<StatementInfo>& statementList, size_t pageCountTarget, bool includeExtras)
{
    std::vector<StatementInfo> subset;
    for (int i = 0; i < statementList.size(); ++i) {
        size_t pageCount = statementList[i].pageCount;
        if (pageCount == pageCountTarget || (includeExtras && pageCount > pageCountTarget))
            subset.push_back(statementList[i]);
    }
    
    return subset;
}


static CFURLRef CreateFileName(CFURLRef destinationFolderUrl, CFStringRef baseFilename, CFStringRef suffix)
{
    CFMutableStringRef fileName = CFStringCreateMutableCopy(kCFAllocatorDefault, 0, baseFilename);
    CFStringAppend(fileName, suffix);
    CFStringAppend(fileName, CFSTR(".pdf"));
    
    CFURLRef fullName = CFURLCreateCopyAppendingPathComponent(kCFAllocatorDefault, destinationFolderUrl, fileName, false);
    
    CFRelease(fileName);
    
    return fullName;
}


static int CreateFile(CFURLRef destinationUrl, CGPDFDocumentRef document, const std::vector<StatementInfo>& statementList)
{
    int destinationPageCount = 0;
    
    CGContextRef writeContext = NULL;
    for (size_t i = 0; i < statementList.size(); ++i)
    {
        StatementInfo statement = statementList[i];
        for (size_t pageIndex = 0; pageIndex < statement.pageCount; ++pageIndex)
        {
            size_t sourcePageNumber = statement.startPageIndex + pageIndex + 1;
            CGPDFPageRef page = CGPDFDocumentGetPage(document, sourcePageNumber);
            if (page != NULL)
            {
                if (writeContext == NULL) {
                    CGRect mediaBox = CGPDFPageGetBoxRect(page, kCGPDFMediaBox);
                    writeContext = CGPDFContextCreateWithURL(destinationUrl, &mediaBox, NULL);
                    if (writeContext == NULL)
                        return -1;
                }
                
                CGPDFContextBeginPage(writeContext, NULL);
                CGContextDrawPDFPage(writeContext, page);
                CGPDFContextEndPage(writeContext);
                
                ++destinationPageCount;
            }
        }
    }

    if (writeContext != NULL)
    {
        CGPDFContextClose(writeContext);
        CGContextRelease(writeContext);
    }
    
    return destinationPageCount;
}


