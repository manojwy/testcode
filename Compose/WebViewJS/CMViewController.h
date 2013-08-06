//
//  CMViewController.h
//  WebViewJS
//
//  Created by suhail on 2/8/13.
//  Copyright (c) 2013 webyog. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "CMWebViewHelper.h"

//@interface UIWebBrowserView : UIView
//@end
//
//@interface UIWebBrowserView (UIWebBrowserView_Additions)
//@end
//
//@implementation UIWebBrowserView (UIWebBrowserView_Additions)
//
//- (id)inputAccessoryView {
//    return nil;
//}
//
//@end

@interface CMViewController : UIViewController <UIWebViewDelegate,UITextFieldDelegate>

    
@property (weak, nonatomic) IBOutlet UIWebView *bodyWebview;
@property (weak, nonatomic) IBOutlet UIScrollView *mainScrollView;
@property (weak, nonatomic) IBOutlet UITextField *textField;
@property (weak, nonatomic) IBOutlet UIButton *buttonToDismiss;


@property (retain, nonatomic) CMWebViewHelper * bodyWebviewHelper;

@end
