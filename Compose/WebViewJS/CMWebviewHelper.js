
function CMWebViewHelper() {
    this.visibleHeight = 64; //Set default height
    this.yOffset = 0;
    this.xOffset = 0;
    this.editor = null;
    this.height = 0;
};

CMWebViewHelper.prototype.CMLog = function(msg) {
    window.external.notify("{\"msg\":\"" + encodeURIComponent(msg) + "\"}");
};

CMWebViewHelper.prototype.CMSendMessage = function(jsonmsg) {
    window.external.notify(jsonmsg);
};

CMWebViewHelper.prototype.HandleKeyup = function () {
    var m;
    m = "Keyup-" + this.height + "--" + this.editor.scrollHeight;
    this.CMLog(m);
    if (this.height != this.editor.scrollHeight) {
        this.height = this.editor.scrollHeight;
        var msg = {
        action:"HeightChange",
        scrollHeight:this.editor.scrollHeight,
        posY:this.getCurrentYPos(),
        scrollTop:0
        };
        this.CMSendMessage(JSON.stringify(msg));
    }
    
};

CMWebViewHelper.prototype.HandleTap = function () {
    //document.body.scrollTop = 0;
    window.setTimeout(function() {
    var msg = {
        action:"Tap",
        scrollHeight:cmWebViewHelper.editor.scrollHeight,
        posY:cmWebViewHelper.getCurrentYPos(),
        scrollTop:document.body.scrollTop
    };
    cmWebViewHelper.CMSendMessage(JSON.stringify(msg));
                      
                      }, 10);
                      
};

CMWebViewHelper.prototype.HandleLoad = function (obj) {
    try {
        obj.editor = document.getElementById("editor");
        obj.currentY =  obj.getCurrentYPos();
        obj.height = this.editor.scrollHeight;
        
        // helper to adjust height on adding new lines as well as when removing new lines
        obj.editor.addEventListener("keyup", function(){obj.HandleKeyup();}, false);
        
        // helper to adjust body scrolltop on tapping;
        obj.editor.addEventListener("click", function(){obj.HandleTap();}, false);
        var msg = {
            action:"Load",
            scrollHeight:obj.editor.scrollHeight,
            posY:obj.getCurrentYPos(),
            scrollTop:0
        };
        obj.CMSendMessage(JSON.stringify(msg));
    } catch (e) {
        obj.CMLog("HandleLoad_" + e);
    }

};

CMWebViewHelper.prototype.setVisibleHeight = function(height) {
    try {
        this.visibleHeight = height;
    } catch(e) {
        this.CMLog("setVisibleHeight_" + e);
    }
};

    // Fetch new mail content
CMWebViewHelper.prototype.getComposeContent = function() {
    try {
        var content = this.editor.innerHTML;
        return content;
    } catch (e) {
        this.CMLog("getComposeContent_" + e);
        return ""
    }
};

// this is used to get the current coordinates of the selection - not very efficient, so it shouldn't be called too often
CMWebViewHelper.prototype.updateOffset = function() {
    try{
        var sel = window.getSelection();
        range = sel.getRangeAt(0);
        tmpSpan = document.createElement('span');
        
        range.insertNode(tmpSpan);
        this.yOffset = tmpSpan.offsetTop;
        this.xOffset = tmpSpan.offsetLeft;
        tmpSpan.parentNode.removeChild(tmpSpan);
    }
    catch(e){
        this.CMLog("updateOffset_" + e);
    }
}

CMWebViewHelper.prototype.getCurrentYPos = function() {
    var x = 0, y = 0;
    var sel = window.getSelection();
    if (sel.rangeCount) {
        var range = sel.getRangeAt(0);
        if (range.getClientRects) {
            var rects = range.getClientRects();
            if (rects.length > 0) {
                x = rects[0].left;
                y = rects[0].top;
            }
        }
    }
    
    if (y ==0) {
        this.updateOffset();
        return this.yOffset;
    }
    
    return y;
}

// editor is the div with 'contenteditable', while visibleHeight is an int,
// set from objective-c (depending on where the webview is positioned, keyboard height and screen height)
CMWebViewHelper.prototype.scrollToVisible = function(){
    try {

        var newYPos = this.getCurrentYPos();
        // don't do anything is Y pos is same
        if (this.currentY == newYPos) {
            return;
        }
        this.currentY = newYPos;
        
        if(this.editor.clientHeight > this.visibleHeight) {
            this.updateOffset();
            if(self.yOffset < window.pageYOffset) {
                window.scrollTo(0, this.yOffset);
            }
            else if(this.yOffset - window.pageYOffset > this.visibleHeight){
                window.scrollTo(0, this.yOffset - this.visibleHeight);
            }
        }
        this.editorScrollHeight = this.editor.scrollHeight;
    }
    catch (e){
       this.CMLog("scrollToVisible_" + e);
    }
}

CMWebViewHelper.prototype.resetScroll = function() {
    //document.body.scrollTop = 0;
    //window.pageYOffset = 0;
    window.scrollTo(0,0);
}

CMWebViewHelper.prototype.test = function() {
    cmWebViewHelper.CMLog(document.body.scrollTop);
}

var cmWebViewHelper = new CMWebViewHelper();
window.onload = function() {cmWebViewHelper.HandleLoad(cmWebViewHelper)};

