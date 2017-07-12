//
//  SearchPDF.h
//
//  Created by Ingage Partners on 7/11/17.
//  Copyright © 2017 Crossroads. All rights reserved.
//

#ifndef SearchPDF_h
#define SearchPDF_h

#include <ApplicationServices/ApplicationServices.h>
#include <vector>

bool FindPagesWithText(CGPDFDocumentRef document, CFStringRef searchText, std::vector<size_t>& pageIndex);

#endif /* SearchPDF_h */
