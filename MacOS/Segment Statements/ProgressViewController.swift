//
//  ProgressViewController.swift
//
//  Created by Ingage Partners on 7/11/17.
//  Copyright © 2017 Crossroads. All rights reserved.
//

import Cocoa
import Foundation

class ProgressViewController: NSViewController {
    
    @IBOutlet weak var statusLabel: NSTextField!
    @IBOutlet weak var progressBar: NSProgressIndicator!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // Do any additional setup after loading the view.
        
        progressBar.usesThreadedAnimation = true
        progressBar.startAnimation(nil)
    }
    
    override func viewDidAppear() {
        super.viewDidAppear()
        
        view.window!.styleMask.remove(NSWindowStyleMask.resizable)
    }
    
    override var representedObject: Any? {
        didSet {
            // Update the view, if already loaded.
        }
    }
    
}

