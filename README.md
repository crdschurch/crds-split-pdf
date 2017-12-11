# crds-split-pdf
This is a utility program for MacOS that will read a PDF file containing year-end Contribution Statements and generate new PDF files that contain the statements grouped based on page count.  For example, if you provide a PDF file named *2017 Statements.pdf* the program will generate the following files:

* *2017 Statements 1-Page.pdf* - Contains all 1-page statements
* *2017 Statements 2-Page.pdf* - Contains all 2-page statements
* *2017 Statements 3-Page.pdf* - Contains all 3-page statements
* *2017 Statements 4-Page.pdf* - Contains all statements with 4 or more pages

### Runtime requirements
MacOS 10.12 or later

The latest build is committed to this repository in the `/dist` folder.  To run the program, download the `SegmentStatements.zip` file to your Mac, double-click to unzip, then double click the `Segment Statements` application bundle to start the application.

### Development notes
The project was built with Xcode 8.3.3.  Since we don't currently have TeamCity build agents that are capable of supporting Xcode projects, the compiled output is included in the `/dist` folder in this repository (bundled as a single .zip file).  The Xcode project includes a Scheme named **Segment Statements** that is appropriate for building/running/debugging the application, and also includes a second Scheme named **Segment Statements ZIP** that will automatically build a Release version of the application, then create/update the `SegmentStatements.zip` file in the `/dist` folder.

To run/debug normally:
1. Set the Scheme to **Segment Statements**
2. Choose **Run** / **Build** / **Profile** / etc. from the **Product** menu.

To create the `/dist/SegmentStatements.zip` file:
1. Set the Scheme to **Segment Statements ZIP**
2. Choose **Build** from the **Product** menu.  The .zip file will be created/updated automatically.
3. Commit and push to github
