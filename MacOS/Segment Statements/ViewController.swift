//
//  ViewController.swift
//
//  Created by Ingage Partners on 7/11/17.
//  Copyright © 2017 Crossroads. All rights reserved.
//

import Cocoa
import Foundation

class ViewController: NSViewController {

    @IBOutlet weak var inputFileText: NSTextField!
    @IBOutlet weak var outputFolderText: NSTextField!
    
    private var _sourceFile:URL?;
    private var _destFolder:URL?;
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
    }

    override var representedObject: Any? {
        didSet {
        // Update the view, if already loaded.
        }
    }


    @IBAction func chooseFileClicked(_ sender: Any) {
        let dialog = NSOpenPanel();
        dialog.showsResizeIndicator    = true;
        dialog.showsHiddenFiles        = false;
        dialog.canChooseDirectories    = false;
        dialog.canCreateDirectories    = true;
        dialog.allowsMultipleSelection = false;
        dialog.allowedFileTypes        = ["pdf"];
        
        if (dialog.runModal() == NSModalResponseOK) {
            _sourceFile = dialog.url;
            
            if (_sourceFile != nil) {
                let path = _sourceFile!.path;
                inputFileText.stringValue = path;
            }
        }
    }
    
    @IBAction func chooseFolderClicked(_ sender: Any) {
        let dialog = NSOpenPanel();
        dialog.showsResizeIndicator    = true;
        dialog.showsHiddenFiles        = false;
        dialog.canChooseDirectories    = true;
        dialog.canChooseFiles          = false;
        dialog.canCreateDirectories    = true;
        dialog.allowsMultipleSelection = false;
        
        if (dialog.runModal() == NSModalResponseOK) {
            _destFolder = dialog.url;
            
            if (_destFolder != nil) {
                let path = _destFolder!.path;
                outputFolderText.stringValue = path;
            }
        }
    }
    
    @IBAction func createSegmentsClicked(_ sender: Any) {
        
        if (_sourceFile == nil) {
            showError(message: "Missing File", information: "You must select an existing PDF for the Input file.");
            return;
        }
        
        if (!isValidPDF(url: _sourceFile!)) {
            showError(message: "Invalid Input File", information: "The Input file is not a valid PDF file.");
            return;
        }

        if (_destFolder == nil) {
            showError(message: "Missing Folder", information: "You must select an existing folder for the Output location.");
            return;
        }
        
        if (!isValidFolder(folder: _destFolder!)) {
            showError(message: "Invalid Folder", information: "The Output folder does not exist.");
            return;
        }
        
        let sourceFile:NSURL = _sourceFile! as NSURL;
        let destFolder:NSURL = _destFolder! as NSURL;

        let baseName:String? = sourceFile.deletingPathExtension?.lastPathComponent;
        if (baseName == nil) {
            showError(message: "Output", information: "Unable to determine the base filename for output files.");
            return;
        }

        // show progress view
        let progressViewController = self.storyboard!.instantiateController(withIdentifier: "ProgressViewController") as! ProgressViewController
        self.presentViewControllerAsSheet(progressViewController)
        
        // process files on a background thread
        DispatchQueue.global(qos: .background).async {
            let numStatements:Int32 = ProcessPDF(sourceFile, destFolder, baseName! as CFString);

            DispatchQueue.main.async {
                self.dismissViewController(progressViewController);

                if (numStatements >= 0) {
                    self.showInfo(message: "Completed.", information: "Processed \(numStatements) statements.");
                } else {
                    self.showError(message: "Failed.", information: "An error occurred while processing PDF files.");
                }
            }
        }
    }
    
    func isValidPDF(url: URL) -> Bool {
        var isValid:Bool = false;
        
        let pdf: CGPDFDocument? = CGPDFDocument(url as CFURL);
        if (pdf != nil) {
            isValid = true;
        }

        return isValid;
    }

    func isValidFolder(folder:URL) -> Bool {
        var isValid:Bool = false;
        
        let fileManager = FileManager.default
        var isDir : ObjCBool = false
        if fileManager.fileExists(atPath: folder.path, isDirectory:&isDir) {
            if isDir.boolValue {
                isValid = true  // file exists and is a directory
            }
        }
        
        return isValid;
    }
    
    func showInfo(message:String, information:String = "") {
        showMessage(message: message, information: information, alertStyle: NSAlertStyle.informational);
    }

    func showError(message:String, information:String = "") {
        showMessage(message: message, information: information, alertStyle: NSAlertStyle.critical);
    }

    func showMessage(message:String, information:String = "", alertStyle:NSAlertStyle) {
        let alert = NSAlert();
        alert.messageText = message;
        alert.informativeText = information;
        alert.alertStyle = alertStyle;
        alert.addButton(withTitle: "OK");

        alert.beginSheetModal(for: self.view.window!, completionHandler: nil);
    }

}

