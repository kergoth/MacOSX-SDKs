/*
        NSGraphicsContext.h
        Application Kit
        Copyright (c) 1997-2014, Apple Inc.
        All rights reserved.
*/

#import <AppKit/AppKitDefines.h>
#import <AppKit/NSGraphics.h>
#import <Foundation/NSGeometry.h>
#import <Foundation/NSObject.h>

#import <CoreGraphics/CGContext.h>

@class NSDictionary;
@class NSString;
@class NSWindow;
@class NSBitmapImageRep;

/* Attributes that can be passed to graphicsContextWithAttributes: method */
APPKIT_EXTERN NSString *NSGraphicsContextDestinationAttributeName; // Can be an instance of NSWindow, NSMutableData, NSBitmapImageRep, or NSURL.

// Attributes for NSMutableData or NSURL destinations
APPKIT_EXTERN NSString *NSGraphicsContextRepresentationFormatAttributeName; // Specifies destination file format

// Supported file format
APPKIT_EXTERN NSString *NSGraphicsContextPSFormat;
APPKIT_EXTERN NSString *NSGraphicsContextPDFFormat;

typedef NS_ENUM(NSUInteger, NSImageInterpolation) {
   NSImageInterpolationDefault = 0,
   NSImageInterpolationNone = 1,
   NSImageInterpolationLow = 2, /* Low quality, fast interpolation. */
   NSImageInterpolationMedium NS_ENUM_AVAILABLE_MAC(10_6) = 4, /* Medium quality, slower than NSImageInterpolationLow. */
   NSImageInterpolationHigh = 3 /* Highest quality, slower than NSImageInterpolationMedium. */
};

@interface NSGraphicsContext : NSObject {
}

// Instantiates from an appropriate concrete subclass depending on NSGraphicsContextDestinationAttributeName attribute
+ (NSGraphicsContext *)graphicsContextWithAttributes:(NSDictionary *)attributes;

// Convenience cover method for [NSGraphicsContext graphicsContextWithAttributes:[NSDictionary dictionaryWithObject:<NSWindow instance> forKey:NSGraphicsContextDestinationAttributeName]]
+ (NSGraphicsContext *)graphicsContextWithWindow:(NSWindow *)window;

/* Convenience cover method for [NSGraphicsContext graphicsContextWithAttributes:[NSDictionary dictionaryWithObject:<NSBitmapImageRep instance> forKey:NSGraphicsContextDestinationAttributeName]]. Only NSBitmapImageRep instances with isPlanar == NO are supported.
*/
+ (NSGraphicsContext *)graphicsContextWithBitmapImageRep:(NSBitmapImageRep *)bitmapRep;

/* Instantiates from a CGContextRef.  initialFlippedState is used to determine the return value from -isFlipped when no view is focused in the context.
*/
+ (NSGraphicsContext *)graphicsContextWithCGContext:(CGContextRef)graphicsPort flipped:(BOOL)initialFlippedState NS_AVAILABLE_MAC(10_10);

// Setting and identifying the current context in the thread
+ (NSGraphicsContext *)currentContext;
+ (void)setCurrentContext:(NSGraphicsContext *)context;

// Convenience cover method for [[NSGraphicsContext currentContext] isDrawingToScreen]
+ (BOOL)currentContextDrawingToScreen;

// Graphics state controls
// Calls -saveGraphicsContext, and pushes the current context on per-thread stack. It's functional equivalent of PSgsave
+ (void)saveGraphicsState;
// Pops a context from per-thread stack, makes it current, and calls -restoreGraphicsContext. It's functional equivalent of PSgrestore
+ (void)restoreGraphicsState;

// Returns attributes used to create this instance
@property (readonly, copy) NSDictionary *attributes;

// Testing the drawing destination
@property (getter=isDrawingToScreen, readonly) BOOL drawingToScreen;

// Controlling the context
- (void)saveGraphicsState;
- (void)restoreGraphicsState;

- (void)flushGraphics;

@property (readonly) CGContextRef CGContext NS_RETURNS_INNER_POINTER NS_AVAILABLE_MAC(10_10);

/* Returns the flip state of the receiver.  The state is determined by messaging -isFlipped to the focus view in the context.  If no view has focus, returns NO unless the receiver is instantiated via graphicsContextWithGraphicsPort:drawingToScreen:flipped: with initialFlippedState == YES.
*/
@property (getter=isFlipped, readonly) BOOL flipped;
@end

@interface NSGraphicsContext(NSGraphicsContext_RenderingOptions)

@property BOOL shouldAntialias;
@property NSImageInterpolation imageInterpolation;

@property NSPoint patternPhase;
@property NSCompositingOperation compositingOperation;
@property NSColorRenderingIntent colorRenderingIntent NS_AVAILABLE_MAC(10_5);
@end

@class CIContext;
@interface NSGraphicsContext (NSQuartzCoreAdditions)
@property (readonly, strong) CIContext *CIContext;
@end

@interface NSGraphicsContext (NSGraphicsContextDeprecated)

/* This method does nothing and is deprecated
 */
+ (void)setGraphicsState:(NSInteger)gState NS_DEPRECATED_MAC(10_0, 10_10);

- (id)focusStack NS_DEPRECATED_MAC(10_0, 10_6);
- (void)setFocusStack:(id)stack NS_DEPRECATED_MAC(10_0, 10_6);

// Platform specific graphics ports (usually CGContextRef.) Use +graphicsContextWithCGContext:flipped: and -CGContext instead.
+ (NSGraphicsContext *)graphicsContextWithGraphicsPort:(void *)graphicsPort flipped:(BOOL)initialFlippedState;
@property (readonly) void *graphicsPort NS_RETURNS_INNER_POINTER;

@end
