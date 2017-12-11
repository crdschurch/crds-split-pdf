//
//  ProcessPDF.h
//
//  Created by Ingage Partners on 7/11/17.
//  Copyright © 2017 Crossroads. All rights reserved.
//

#ifndef ProcessPDF_h
#define ProcessPDF_h

#include <ApplicationServices/ApplicationServices.h>

#ifdef __cplusplus
extern "C" {
#endif
    
int ProcessPDF(CFURLRef inputFile, CFURLRef outputPath, CFStringRef baseName);

#ifdef __cplusplus
}
#endif
    
#endif /* ProcessPDF_h */
