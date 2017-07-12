# crds-split-pdf
This is a utility program for MacOS that will read a PDF file containing year-end Contribution Statements and generate new PDF files that contain the statements grouped based on page count.  For example, if you provide a PDF file named *2017 Statements.pdf* the program will generate the following files:

* *2017 Statements 1-Page.pdf* - Contains all 1-page statements
* *2017 Statements 2-Page.pdf* - Contains all 2-page statements
* *2017 Statements 3-Page.pdf* - Contains all 3-page statements
* *2017 Statements 4-Page.pdf* - Contains all statements with 4 or more pages

### Runtime requirements
MacOS 10.12 or later
