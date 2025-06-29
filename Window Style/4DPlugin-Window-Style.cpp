/* --------------------------------------------------------------------------------
 #
 #  4DPlugin-Window-Style.cpp
 #	source generated by 4D Plugin Wizard
 #	Project : Window Style
 #	author : miyako
 #	2021/04/23
 #  
 # --------------------------------------------------------------------------------*/

#include "4DPlugin-Window-Style.h"

#pragma mark -

void PluginMain(PA_long32 selector, PA_PluginParameters params) {
    
	try
	{
        switch(selector)
        {
			// --- Window Style
            
			case 1 :
                SET_WINDOW_STYLE(params);
				break;

        }

	}
	catch(...)
	{

	}
}

#pragma mark -

static PA_Picture ob_get_p(PA_ObjectRef obj, const wchar_t *_key) {

    PA_Picture p = NULL;
    
    bool is_defined = false;
    
    if(obj)
    {
        CUTF16String ukey;
        json_wconv(_key, &ukey);
        PA_Unistring key = PA_CreateUnistring((PA_Unichar *)ukey.c_str());
        is_defined = PA_HasObjectProperty(obj, &key);
        
        if(is_defined)
        {
            PA_Variable v = PA_GetObjectProperty(obj, &key);
            if(PA_GetVariableKind(v) == eVK_Picture)
            {
                p = PA_GetPictureVariable(v);
            }
        }
    }
    
    return p;
}

typedef struct {
    NSWindow *window;
    NSColor *color;
} setBackgroundColor_t;

static void setBackgroundColor(setBackgroundColor_t *params) {
    
    [[params->window contentView] setWantsLayer:NO];
    [[params->window contentView] layer].contents = nil;
    
    [params->window setBackgroundColor:params->color];
}

typedef struct {
    NSWindow *window;
    BOOL opaque;
} setOpaque_t;

static void setOpaque(setOpaque_t *params) {
    
    [params->window setOpaque:params->opaque];
}

typedef struct {
    NSWindow *window;
    BOOL titlebarAppearsTransparent;
} setTitlebarAppearsTransparent_t;

static void setTitlebarAppearsTransparent(setTitlebarAppearsTransparent_t *params) {
 
    if(params->titlebarAppearsTransparent) {
        [params->window setStyleMask:[params->window styleMask]| NSWindowStyleMaskFullSizeContentView];
    }else{
        [params->window setStyleMask:[params->window styleMask]&~NSWindowStyleMaskFullSizeContentView];
    }
    
    [params->window setTitlebarAppearsTransparent:params->titlebarAppearsTransparent];
}

typedef struct {
    NSWindow *window;
    BOOL visible;
} setVisible_t;

static void setVisible(setVisible_t *params) {
        
    //on hides the title string
    if(params->visible){
        [params->window setTitleVisibility:NSWindowTitleVisible];
    }else{
        [params->window setTitleVisibility:NSWindowTitleHidden];
        
        //only disables the button
        /*
        [params->window setStyleMask:[params->window styleMask]&~NSWindowStyleMaskClosable];
        [params->window setStyleMask:[params->window styleMask]&~NSWindowStyleMaskMiniaturizable];
        [params->window setStyleMask:[params->window styleMask]&~NSWindowStyleMaskResizable];
        */
        
        //crash
         /*
          [params->window setStyleMask:[params->window styleMask]&~NSWindowStyleMaskTitled];
        */
    }

}

typedef struct {
    NSWindow *window;
    BOOL movableByWindowBackground;
} setMovableByWindowBackground_t;

static void setMovableByWindowBackground(setMovableByWindowBackground_t *params) {
 
    [params->window setMovableByWindowBackground:params->movableByWindowBackground];
}

typedef struct {
    NSWindow *window;
    NSImage *image;
} setContentViewImage_t;

static void setContentViewImage(setContentViewImage_t *params) {
    
    if(params->image) {
        [[params->window contentView] setWantsLayer:YES];
        [[params->window contentView] layer].contents = params->image;
    }else{
        [[params->window contentView] setWantsLayer:NO];
        [[params->window contentView] layer].contents = nil;
    }
}

typedef struct {
    NSWindow *window;
    BOOL activate;
    NSInteger state;
    NSInteger material;
    NSInteger blendingMode;
    BOOL emphasized;
} setVibrance_t;

// https://developer.apple.com/design/human-interface-guidelines/macos/visual-design/translucency/
static void setVibrance(setVibrance_t *params) {
    NSArray* subviews = [[params->window contentView] subviews];

    // remove any previous vibrancy view
    NSArray *visualEffectViews = [subviews filteredArrayUsingPredicate:[NSPredicate predicateWithBlock:^BOOL(id view, NSDictionary *bindings) {
        return [view isKindOfClass: NSVisualEffectView.self];
    }]];
    for (NSView* view in visualEffectViews) {
        [view removeFromSuperview];
    }

    if(params->activate) {
        // add the special view bellow to activate vibrancy
        NSVisualEffectView* effectView = [NSVisualEffectView new];
        effectView.translatesAutoresizingMaskIntoConstraints = NO;
        effectView.state = (NSVisualEffectState) params->state;
        effectView.material = (NSVisualEffectMaterial) params->material;
        effectView.emphasized = params->emphasized;
        effectView.blendingMode = (NSVisualEffectBlendingMode) params->blendingMode;
        // add bellow first view
        NSView* firstView = subviews.firstObject;
        if(firstView==nil) {
            [[params->window contentView] addSubview:effectView];
        } else {
            [[params->window contentView] addSubview:effectView positioned: NSWindowBelow relativeTo: firstView];
        }
        // constraints to keep fullscreen with window
        [effectView.leadingAnchor constraintEqualToAnchor: [params->window contentView].leadingAnchor].active = YES;
        [effectView.trailingAnchor constraintEqualToAnchor: [params->window contentView].trailingAnchor].active = YES;
        [effectView.topAnchor constraintEqualToAnchor: [params->window contentView].topAnchor].active = YES;
        [effectView.bottomAnchor constraintEqualToAnchor: [params->window contentView].bottomAnchor].active = YES;
    }
}


typedef struct {
	NSWindow *window;
	BOOL activate;
	CGFloat cornerRadius;
	NSColor* tintColor;
	CGFloat padding;
} setGlass_t;

// https://developer.apple.com/documentation/appkit/nsglasseffectview
static void setGlass(setGlass_t *params) {
	NSArray* subviews = [[params->window contentView] subviews];

	if (@available(macOS 26.0, *)) {
		// remove any previous vibrancy view
		NSArray *effectViews = [subviews filteredArrayUsingPredicate:[NSPredicate predicateWithBlock:^BOOL(id view, NSDictionary *bindings) {
			return [view isKindOfClass: NSGlassEffectView.self];
		}]];
		for (NSView* view in effectViews) {
			[view removeFromSuperview];
		}

		if(params->activate) {
			// add the special view bellow to activate glass effect
			NSGlassEffectView* effectView = [NSGlassEffectView new];
			effectView.translatesAutoresizingMaskIntoConstraints = NO;
			effectView.cornerRadius = params->cornerRadius;
			effectView.tintColor = params->tintColor;
			// add bellow first view
			NSView* firstView = subviews.firstObject;
			if(firstView==nil) {
				[[params->window contentView] addSubview:effectView];
			} else {
				[[params->window contentView] addSubview:effectView positioned: NSWindowBelow relativeTo: firstView];
			}
			// constraints to keep fullscreen with window
			if (params->padding == 0) {
				[effectView.leadingAnchor constraintEqualToAnchor: [params->window contentView].leadingAnchor].active = YES;
				[effectView.trailingAnchor constraintEqualToAnchor: [params->window contentView].trailingAnchor].active = YES;
				[effectView.topAnchor constraintEqualToAnchor: [params->window contentView].topAnchor].active = YES;
				[effectView.bottomAnchor constraintEqualToAnchor: [params->window contentView].bottomAnchor].active = YES;
			}
			else {
				[effectView.leadingAnchor constraintEqualToAnchor: [params->window contentView].leadingAnchor constant: params->padding].active = YES;
				[effectView.trailingAnchor constraintEqualToAnchor: [params->window contentView].trailingAnchor constant: -params->padding].active = YES;
				[effectView.topAnchor constraintEqualToAnchor: [params->window contentView].topAnchor constant: params->padding].active = YES;
				[effectView.bottomAnchor constraintEqualToAnchor: [params->window contentView].bottomAnchor constant: -params->padding].active = YES;
			}
		}
	}
}


void SET_WINDOW_STYLE(PA_PluginParameters params) {

    PA_long32 arg1 = PA_GetLongParameter(params, 1);
    
    NSWindow *window = reinterpret_cast<NSWindow *>(PA_GetWindowPtr(reinterpret_cast<PA_WindowRef>(arg1)));
    
    if(window) {
                
        PA_ObjectRef options = PA_GetObjectParameter(params, 2);
        if(options) {
            
            if(ob_is_defined(options, L"backgroundColor")) {
                PA_ObjectRef backgroundColor = ob_get_o(options, L"backgroundColor");
                if(backgroundColor) {
                    CGFloat r, g, b, a;
                    r = ob_get_n(backgroundColor, L"red");
                    g = ob_get_n(backgroundColor, L"green");
                    b = ob_get_n(backgroundColor, L"blue");
                    a = ob_get_n(backgroundColor, L"alpha");
                    NSColor *color = [NSColor colorWithRed:r green:g blue:b alpha:a];
                    color = [color colorUsingColorSpace:[NSColorSpace displayP3ColorSpace]];

                    setBackgroundColor_t params;
                    params.window = window;
                    params.color  = color;
                    
                    PA_RunInMainProcess((PA_RunInMainProcessProcPtr)setBackgroundColor, &params);
                    
                }
            }
            
            if(ob_is_defined(options, L"backgroundImage")) {
                PA_Picture backgroundImage = ob_get_p(options, L"backgroundImage");
                if(backgroundImage) {
                    @autoreleasepool {
                        CGImageRef ci = (CGImageRef)PA_CreateNativePictureForScreen(backgroundImage);if(ci)
                        {
                            setContentViewImage_t params;
                            params.window = window;
                            params.image = [[NSImage alloc]initWithCGImage:ci size:NSZeroSize];
                            
                            PA_RunInMainProcess((PA_RunInMainProcessProcPtr)setContentViewImage, &params);
                        }
                    }
                }else{
                    
                    setContentViewImage_t params;
                    params.window = window;
                    params.image = nil;
                    
                    PA_RunInMainProcess((PA_RunInMainProcessProcPtr)setContentViewImage, &params);
                }
            }
            
            if(ob_is_defined(options, L"backgroundImagePath")) {
                CUTF8String backgroundImagePath;
                if(ob_get_s(options, L"backgroundImagePath", &backgroundImagePath)){
                    @autoreleasepool {
                        NSString *u8 = [NSString stringWithUTF8String:(const char *)backgroundImagePath.c_str()];
                        NSURL *u = (NSURL *)CFURLCreateWithFileSystemPath(kCFAllocatorDefault, (CFStringRef)u8, kCFURLHFSPathStyle, false);
                        if(u){
                            
                            setContentViewImage_t params;
                            params.window = window;
                            params.image = [[NSImage alloc]initWithContentsOfURL:u];
                            
                            PA_RunInMainProcess((PA_RunInMainProcessProcPtr)setContentViewImage, &params);
                        }
                    }
                }
            }
            
            if(ob_is_defined(options, L"movableByWindowBackground")) {
                BOOL movableByWindowBackground = ob_get_b(options, L"movableByWindowBackground");
                
                setMovableByWindowBackground_t params;
                params.window = window;
                params.movableByWindowBackground = movableByWindowBackground;
                
                PA_RunInMainProcess((PA_RunInMainProcessProcPtr)setMovableByWindowBackground, &params);
            }
            
            if(ob_is_defined(options, L"titlebarAppearsTransparent")) {
                BOOL titlebarAppearsTransparent = ob_get_b(options, L"titlebarAppearsTransparent");
                 
                setTitlebarAppearsTransparent_t params;
                params.window = window;
                params.titlebarAppearsTransparent = titlebarAppearsTransparent;
                
                PA_RunInMainProcess((PA_RunInMainProcessProcPtr)setTitlebarAppearsTransparent, &params);
            }
            
            if(ob_is_defined(options, L"opaque")) {
                BOOL opaque = ob_get_b(options, L"opaque");
                
                setOpaque_t params;
                params.window = window;
                params.opaque  = opaque;
                            
                PA_RunInMainProcess((PA_RunInMainProcessProcPtr)setOpaque, &params);
            }
            
            if(ob_is_defined(options, L"titlebarVisible")) {
                BOOL visible = ob_get_b(options, L"titlebarVisible");
                
                setVisible_t params;
                params.window = window;
                params.visible  = visible;
                            
                PA_RunInMainProcess((PA_RunInMainProcessProcPtr)setVisible, &params);
            }

            if(ob_is_defined(options, L"vibrance")) {
                PA_ObjectRef vibrance = ob_get_o(options, L"vibrance");

                setVibrance_t params;
                params.window = window;
                params.state = 0;
                params.material = 0;
                params.blendingMode = 0;
                params.emphasized = false;
                if (vibrance==nil) {
                    params.activate = ob_get_b(options, L"vibrance"); // just activate or deactivate it
                } else {
                    params.activate = true;
                    params.state = ob_get_n(vibrance, L"state");
                    params.material = ob_get_n(vibrance, L"material");
                    params.blendingMode = ob_get_n(vibrance, L"blendingMode");
                    params.emphasized = ob_get_b(options, L"emphasized");
                }

                PA_RunInMainProcess((PA_RunInMainProcessProcPtr)setVibrance, &params);
            }

            if(ob_is_defined(options, L"glass")) {
                PA_ObjectRef glass = ob_get_o(options, L"glass");

                setGlass_t params;
                params.window = window;
                params.cornerRadius = 0;
                params.tintColor = nil;
                params.padding = 0;
                if (glass==nil) {
                    params.activate = ob_get_b(options, L"glass"); // just activate or deactivate it
                } else {
                    params.activate = true;
                    params.cornerRadius = ob_get_n(glass, L"cornerRadius");
                    params.padding = ob_get_n(glass, L"padding");

                    PA_ObjectRef tintColor = ob_get_o(options, L"tintColor");
                    if(tintColor) {
                        CGFloat r, g, b, a;
                        r = ob_get_n(tintColor, L"red");
                        g = ob_get_n(tintColor, L"green");
                        b = ob_get_n(tintColor, L"blue");
                        a = ob_get_n(tintColor, L"alpha");
                        NSColor *color = [NSColor colorWithRed:r green:g blue:b alpha:a];
                        color = [color colorUsingColorSpace:[NSColorSpace displayP3ColorSpace]];
                        params.tintColor = color;
                    }
                }

                PA_RunInMainProcess((PA_RunInMainProcessProcPtr)setGlass, &params);
            }

        }
        
    }
    
}
