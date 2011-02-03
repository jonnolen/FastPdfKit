//
//  MFDocumentManager.h
//  OffscreenRendererTest
//
//  Created by Nicolò Tosi on 4/20/10.
//  Copyright 2010 MobFarm S.r.l. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIKit.h>
#import "mfprofile.h"

@class MFOffscreenRenderer;

@interface MFDocumentManager : NSObject <UIAlertViewDelegate>{

	MFOffscreenRenderer *renderer;
	
	CGPDFDocumentRef document;
	NSLock * lock;
	NSURL * url;
	
	NSUInteger numberOfPages;
	
	NSString * password;
	
	NSLock * pageDataLock;
	int *dataSetFlags;
	CGRect *cropboxes;
	int *rotations;
	
	MFProfile profile;
}

// These method are used internally.
-(CGImageRef)createImageFromPDFPagesLeft:(NSInteger)leftPage andRight:(NSInteger)rightPage size:(CGSize)size andScale:(CGFloat)scale useLegacy:(BOOL)legacy;
-(CGImageRef)createImageFromPDFPage:(NSInteger)page size:(CGSize)size  andScale:(CGFloat)scale useLegacy:(BOOL)legacy;
-(void)drawPageNumber:(NSInteger)pageNumber onContext:(CGContextRef)ctx;
-(void)getCropbox:(CGRect *)cropbox andRotation:(int *)rotation forPageNumber:(NSInteger)pageNumber;

/**
 Create a thumbnail for a specific page. It will look far better than the thumbnail integrated inside the pdf, but
 it is also slower.
 */
-(CGImageRef)createImageForThumbnailOfPageNumber:(NSUInteger)pageNr ofSize:(CGSize)size andScale:(CGFloat)scale;

/** 
 Factory method to create an MFDocumentManager instance from a know file path.
 */
+(MFDocumentManager *)documentManagerWithFilePath:(NSString *)filePath;

/** 
 Return an array of MFOutlineEntry as the outline/TOC of the pdf document.
 */
-(NSMutableArray *)outline;

/**
 Initializer. You can also use the factory method above. 
 */
-(id)initWithFileUrl:(NSURL*)anUrl;
	
/** 
 Check if a document is encrypted and blocked by a password or not.
 */
-(BOOL)isLocked;

/**
 Try to unlock the document with a password and return if the unlock has been successful or not.
*/
-(BOOL)tryUnlockWithPassword:(NSString *)aPassword;

/** 
 Return the number of pages that make up the document.
 */
-(NSUInteger)numberOfPages;

/** 
 Clear the page cache. It is important to call this method on memory warning as in the sample code
 to prevent the application being killed right for excessive memory usage.
 */
-(void)emptyCache;

/**
 Return an array of MFTextItem representing the matches of teh search term on the page passed
 as arguments. It is a good choice running this method in a secondary thread.
 */
-(NSArray *)searchResultOnPage:(NSUInteger)pageNr forSearchTerms:(NSString *)searchTerm;

/**
 Return a string representation of the text contained in a pdf page.
 */
-(NSString *)wholeTextForPage:(NSUInteger)pageNr;

/**
 This is an experimental features. It will allow to customize the behaviour for search and extraction of text. You can
 set the values inside of this struct before launching a search or a text extraction action. Look at mfprofile.h for an explanation of the MFProfile struct and how to customize it.
 */
@property (nonatomic,readwrite) MFProfile profile;

@end