//
//  CMViewController.m
//  WebViewJS
//
//  Created by suhail on 2/8/13.
//  Copyright (c) 2013 webyog. All rights reserved.
//

#import "CMViewController.h"
#import <QuartzCore/QuartzCore.h>



@interface CMViewController () <CMWebViewHelperDelegate, UIScrollViewDelegate>
{
    CGFloat keyboardHeight;
    CGFloat screenHeight;
    CGPoint webviewContentOffset;
}

@end



@implementation CMViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    keyboardHeight = 0;
    screenHeight = 0;

    self.bodyWebview.scrollView.scrollEnabled = NO;
    self.bodyWebview.scrollView.bounces = NO;
    
    self.bodyWebviewHelper = [[CMWebViewHelper alloc] initWithWebView:self.bodyWebview delegate:self webViewDelegate:self];
    
        //HANDLING KEYBOARD NOTIFICATIONS
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillShow:) name:UIKeyboardWillShowNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillHide:) name:UIKeyboardWillHideNotification object:nil];
     [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardDidHide:) name:UIKeyboardDidHideNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardDidShow:) name:UIKeyboardDidShowNotification object:nil];
        
    [self loadEmptyDataInWebView];
    //[self setScrollViewHeight:816];
    
    self.bodyWebview.scrollView.delegate = self;
}

-(void)setScrollViewHeight:(NSInteger)height
{
//    self.mainScrollView.contentSize = CGSizeMake(self.mainScrollView.frame.size.width, height + self.bodyWebview.frame.origin.y  /* Top Space */+ 260 /*KB*/);
    self.mainScrollView.contentSize = CGSizeMake(self.mainScrollView.frame.size.width, height + self.bodyWebview.frame.origin.y );
    CGRect fr = self.bodyWebview.frame;
    fr.size = CGSizeMake(self.bodyWebview.frame.size.width, height);
    self.bodyWebview.frame = fr;
}

- (void)scrollViewDidScroll:(UIScrollView *)scrollView
{
    if(scrollView != self.bodyWebview.scrollView)
    {
        return;
    }
    
    // don;t allow webview scroll to scroll
    CGPoint t = self.bodyWebview.scrollView.contentOffset;
    t.y = 0;
    self.bodyWebview.scrollView.contentOffset = t;
}



- (void)keyboardDidHide:(NSNotification *)notification {
    //NSLog(@"keyboardDidHide");

}


- (void)keyboardDidShow:(NSNotification *)aNotification {
     //NSLog(@"keyboardDidShow");
    self.bodyWebview.scrollView.contentOffset = webviewContentOffset;
    
}

- (void)keyboardWillShow:(NSNotification *)aNotification {
    //NSLog(@"keyboardWillShow");
    
    NSDictionary* keyboardInfo = [aNotification userInfo];
    NSValue* keyboardFrameBegin = [keyboardInfo valueForKey:UIKeyboardFrameBeginUserInfoKey];
    CGRect keyboardFrameBeginRect = [keyboardFrameBegin CGRectValue];
    
    keyboardHeight = keyboardFrameBeginRect.size.height;
    
    // remove the next/prev navigation for webview
    [self performSelector:@selector(removeBar) withObject:nil afterDelay:0];
     webviewContentOffset = self.bodyWebview.scrollView.contentOffset;
    self.mainScrollView.frame = CGRectMake(self.mainScrollView.frame.origin.x, self.mainScrollView.frame.origin.y, 320, self.view.frame.size.height - keyboardHeight + 44 );
}

- (void)keyboardWillHide:(NSNotification *)notification {
   // NSLog(@"keyboardWillHide");
    self.mainScrollView.frame = CGRectMake(self.mainScrollView.frame.origin.x, self.mainScrollView.frame.origin.y, 320, self.view.frame.size.height  );

}


    // removing the topbar on keyboard
- (void)removeBar {
    UIWindow *keyboardWindow = nil;
    for (UIWindow *testWindow in [[UIApplication sharedApplication] windows]) {
        if (![[testWindow class] isEqual:[UIWindow class]]) {
            keyboardWindow = testWindow;
            break;
        }
    }

    for (UIView *possibleFormView in [keyboardWindow subviews]) {
            // iOS 5 sticks the UIWebFormView inside a UIPeripheralHostView.
        if ([[possibleFormView description] rangeOfString:@"UIPeripheralHostView"].location != NSNotFound) {
            for (UIView *subviewWhichIsPossibleFormView in [possibleFormView subviews]) {
                if ([[subviewWhichIsPossibleFormView description] rangeOfString:@"UIWebFormAccessory"].location != NSNotFound) {
                    [subviewWhichIsPossibleFormView removeFromSuperview];
                }
                    // iOS 6 leaves a grey border / shadow above the hidden accessory row
                if ([[subviewWhichIsPossibleFormView description] rangeOfString:@"UIImageView"].location != NSNotFound) {
                        // we need to add the QuartzCore framework for the next line
                    [[subviewWhichIsPossibleFormView layer] setOpacity: 0.0];
                }
            }
        }
    }
}



-(void)loadEmptyDataInWebView
{
    NSString *html;
    NSString *body;
    
    
    body = @"Sent using <a href = 'https://www.cloudmagic.com'>CloudMagic</a>";
    
    NSString * jsBridgeScriptPath = [[NSBundle mainBundle] pathForResource:@"CMIOSJSBridge" ofType:@"js"];
    
    NSError * jsBridgeScriptLoadingError;
    NSString * jsBridgeScript = [[NSString alloc] initWithContentsOfFile:jsBridgeScriptPath encoding:NSUTF8StringEncoding error:&jsBridgeScriptLoadingError];
    
    if(jsBridgeScriptLoadingError)
        {
        return;
        }
    
    NSString * jsWebviewHeightHandlerScriptPath = [[NSBundle mainBundle] pathForResource:@"CMWebviewHelper" ofType:@"js"];
    
    NSError * jsWebviewHandlerScriptLoadError;
    NSString * jsWebviewHeightHandlerScript = [[NSString alloc] initWithContentsOfFile:jsWebviewHeightHandlerScriptPath encoding:NSUTF8StringEncoding error:&jsWebviewHandlerScriptLoadError];
    
    NSString * jsWebviewHelperCssPath = [[NSBundle mainBundle] pathForResource:@"CMWebviewHelper" ofType:@"css"];
    NSString * jsWebviewHelperCss = [[NSString alloc] initWithContentsOfFile:jsWebviewHelperCssPath encoding:NSUTF8StringEncoding error:&jsWebviewHandlerScriptLoadError];
    
    
    if(jsWebviewHandlerScriptLoadError)
        {        
        return;
        }
    
    html = [NSString stringWithFormat:@"<html>\
            <head>\
            <meta name=\"viewport\"  content=\"target-densitydpi=device-dpi,width=270px; width=device-width; initial-scale=1.0; maximum-scale=1.0; user-scalable=no;\" /> \
            <style type=\"text/css\">%@</style>\n\
            <script>%@</script>\n\
            <script>%@</script>\n\
            </head>\
            <body>\
            <div id=\"wrap\">\
            <div id=\"editor\" contenteditable=\"true\">\
            <br/>1<br/>2<br/>3<br/>4<br/>5<br/>6<br/>7<br/>8<br/>9<br/>10\
            <br/>1<br/>2<br/>3<br/>4<br/>5<br/>6<br/>7<br/>8<br/>9<br/>20\
            <br/>1<br/>2<br/>3<br/>4<br/>5<br/>6<br/>7<br/>8<br/>9<br/>30\
            <br/>1<br/>2<br/>3<br/>4<br/>5<br/>6<br/>7<br/>8<br/>9<br/>40\
            <br/>1<br/>2<br/>3<br/>4<br/>5<br/>6<br/>7<br/>8<br/>9<br/>50\
            </div>\
            </body>\
            </html>",
            jsWebviewHelperCss,
            jsBridgeScript,
            jsWebviewHeightHandlerScript];
       
    [self.bodyWebview loadHTMLString:html baseURL:nil];

}

-(void) handleJSOnLoad:(NSDictionary *)notification
{
    NSInteger scrollHeight = [[notification objectForKey:@"scrollHeight"] integerValue];
    [self setScrollViewHeight:scrollHeight];
}

-(void) handleJSOnTap:(NSDictionary *)notification
{
    /////WORKING BLOCK///////////////
    //         NSInteger posY = [[notification objectForKey:@"posY"] integerValue];
    //         CGPoint p = self.mainScrollView.contentOffset;
    //         p.y = posY  + self.bodyWebview.frame.origin.y - 100;
    //         NSLog(@"New P.y:%f", p.y);
    //         [self.mainScrollView setContentOffset:p animated:NO];
    //////////////////////////////////////
    NSInteger posY = [[notification objectForKey:@"posY"] integerValue];
    CGPoint pnt = self.mainScrollView.contentOffset;
    NSInteger webViewStartY = self.bodyWebview.frame.origin.y;
    NSInteger scrollViewStartY = pnt.y;
    NSInteger webViewCursorY = posY;
    NSInteger paddingY = 20;
    NSInteger modifiedKeyboardHeight = keyboardHeight -20; // remove the topbar
    
    screenHeight = self.view.frame.size.height;
    
    NSLog(@"scrollViewStartY: %d, webViewStartY: %d, webViewCursorY: %d", scrollViewStartY, webViewStartY, webViewCursorY);
    NSLog(@"screen Height: %f", screenHeight);
    NSLog(@"Keyboard Height: %f", keyboardHeight);
    
    
    if((webViewCursorY + webViewStartY) < (scrollViewStartY + paddingY)) {
        pnt.y = pnt.y - paddingY;
        [self.mainScrollView setContentOffset:pnt animated:NO];
        return;
    }
    
    
    if ((webViewCursorY + webViewStartY) < (screenHeight - modifiedKeyboardHeight)) {
        return;
    }
    
    if(scrollViewStartY + (screenHeight - modifiedKeyboardHeight) > (webViewCursorY + webViewStartY)) {
        return;
    }
    
    pnt.y = (webViewCursorY - (screenHeight - modifiedKeyboardHeight) + webViewStartY + paddingY /*extra padding*/);
    
    NSLog(@"New pnt: %f", pnt.y);
    [self.mainScrollView setContentOffset:pnt animated:NO];
    //////////////////////////////////////
    
}

-(void)webView:(UIWebView *)webView notificationFromJS:(NSDictionary *)notification
{
    NSLog(@"JS notification: %@", notification);
    NSString *action = [notification objectForKey:@"action"];

    if ([[action lowercaseString] isEqualToString:@"load"]) {
        [self handleJSOnLoad:notification];
        return;
    }
    
     if ([[action lowercaseString] isEqualToString:@"tap"]) {
         [self handleJSOnTap:notification];
         return;
         
     }
    
    if ([[action lowercaseString] isEqualToString:@"heightchange"]) {
        [self handleJSOnLoad:notification];
        [self handleJSOnTap:notification];
    }
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
    NSLog(@"Mem warning");
}
- (IBAction)buttonClicked:(id)sender
{
    [self.textField resignFirstResponder];
}

@end
