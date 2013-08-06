//
//  CMWebViewHelper.m
//  CloudMagic
//
//  Created by suhas on 28/03/13.
//  Copyright (c) 2013 webyog. All rights reserved.
//

#import "CMWebViewHelper.h"
#import "JSONKit.h"

@implementation CMWebViewHelper


-(id)initWithWebView:(UIWebView *)webView delegate:(id <CMWebViewHelperDelegate> )delegate webViewDelegate:(id <UIWebViewDelegate>)webViewDelegate
{
    self = [super init];
    
    if(self)
    {
        self.webView = webView;
        webView.delegate = self;
        self.delegate = delegate;
        self.webViewDelegate = webViewDelegate;
    }
    
    return self;
}

-(void)dealloc
{
    self.webView = nil;
    self.delegate = nil;
    self.webViewDelegate = nil;
    self.documentURLString = nil;
    self.documentTitle = nil;
    self.currentlyLoadingURL = nil;
}

#pragma mark - UIWebView delegates

-(void)webViewDidStartLoad:(UIWebView *)webView
{
    if([self.webViewDelegate respondsToSelector:@selector(webViewDidStartLoad:)] )
    {
        [self.webViewDelegate webViewDidStartLoad:webView];
    }
}

-(void)webViewDidFinishLoad:(UIWebView *)webView
{
    if([self.webViewDelegate respondsToSelector:@selector(webViewDidFinishLoad:)] )
    {
        [self.webViewDelegate webViewDidFinishLoad:webView];
    }
}

-(void)webView:(UIWebView *)webView didFailLoadWithError:(NSError *)error
{
    if([self.webViewDelegate respondsToSelector:@selector(webView:didFailLoadWithError:)] )
    {
        [self.webViewDelegate webView:webView didFailLoadWithError:error];
    }
}

-(BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType
{
    NSURL *url = [request URL];
    
	if ([[url scheme] isEqualToString:@"native"]) {
		
        NSString * args = [[url query] stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        
        dispatch_async(dispatch_get_main_queue(), ^{
            
            if([self.delegate respondsToSelector:@selector(webView:notificationFromJS:)])
            {
                [self.delegate webView:webView notificationFromJS:args.objectFromJSONString];
            }
            
        });
        
        // Tell the JS code that we've gotten this command, and we're ready for another
        [webView stringByEvaluatingJavaScriptFromString:@"EXPOSED_TO_NATIVE.js_ObjC_bridge.ready = true;"];
        
		return NO;
	}
    
    
    if([self.webViewDelegate respondsToSelector:@selector(webView:shouldStartLoadWithRequest:navigationType:)])
    {
        if([self.webViewDelegate webView:webView shouldStartLoadWithRequest:request navigationType:navigationType])
        {
            self.currentlyLoadingURL = url;
            
            return YES;
        }
        else
        {
            return NO;
        }
    }
    else
    {
        self.currentlyLoadingURL = url;
        
        return YES;
    }
}

-(NSString *)documentURLString
{
    _documentURLString = [self.webView stringByEvaluatingJavaScriptFromString:@"document.URL"];
    
    return _documentURLString;
}

-(NSString *)documentTitle
{
    _documentTitle = [self.webView stringByEvaluatingJavaScriptFromString:@"document.title"];
    
    return _documentTitle;
}

@end
