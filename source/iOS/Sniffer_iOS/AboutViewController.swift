//
//  AboutViewController.swift
//  Sniffer_iOS
//
//  Created by Dang Duc Nam on 7/11/18.
//  Copyright © 2018 Axon Active VN. All rights reserved.
//

import UIKit
import WebKit
import MBProgressHUD

class AboutViewController: UIViewController {

//    var spinner: UIActivityIndicatorView!
    var webView: WKWebView!

    override func loadView() {
        super.loadView()
        webView = WKWebView(frame: self.view.frame)
        webView.uiDelegate = self
        self.view.addSubview(webView)

//        spinner = UIActivityIndicatorView(activityIndicatorStyle: .gray)
//        webView.addSubview(spinner)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        self.title = NSLocalizedString("about_view_controller.title", bundle: Bundle.sniffer, comment: "About view controller title")
        
        webView.navigationDelegate = self
        
        MBProgressHUD.showAdded(to: self.view, animated: true)
        
        let bundle = Foundation.Bundle.main
        let baseURL = bundle.url(forResource: "HTML", withExtension: nil)!
        let contentURL = baseURL.appendingPathComponent("About.htm")
        let htmlString = try? String(contentsOf: contentURL, encoding: String.Encoding.utf8)
        self.webView.loadHTMLString(htmlString!, baseURL: baseURL)

//        let myURL = URL(string:AirSnifferServer.aboutURL)
//        let myRequest = URLRequest(url: myURL!)
//        webView.load(myRequest)
        
//        spinner.center = webView.center
//        self.spinner(start: true)
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    

    /*
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }
    */
    
}

extension AboutViewController: WKNavigationDelegate, WKUIDelegate {
    func webView(_ webView: WKWebView, didFinish navigation: WKNavigation!) {
//        self.spinner(start: false)
        MBProgressHUD.hide(for: self.view, animated: false)
    }
}
