// <osiris_sps_source_header>
// This file is part of Osiris Serverless Portal System.
// Copyright (C)2005-2012 Osiris Team (info@osiris-sps.org) / http://www.osiris-sps.org )
//
// Osiris Serverless Portal System is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Osiris Serverless Portal System is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Osiris Serverless Portal System.  If not, see <http://www.gnu.org/licenses/>.
// </osiris_sps_source_header>

#include "stdafx.h"
#include "image.h"

#include "buffer.h"
#include "FreeImage/FreeImage.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class fipObject
{
public:
	virtual BOOL isValid() const = 0;
};

//////////////////////////////////////////////////////////////////////

class fipMemoryIO;

//////////////////////////////////////////////////////////////////////

class fipImage : public fipObject
{
protected:
	/// DIB data
	FIBITMAP *_dib;
	/// TRUE whenever the display need to be refreshed
	mutable BOOL _bHasChanged;

public:
	friend class fipMultiPage;

public:

	/**@name Creation & Destruction */
	//@{
	/**
	Constructor
	@see FreeImage_AllocateT
	*/
	fipImage(FREE_IMAGE_TYPE image_type = FIT_BITMAP, WORD width = 0, WORD height = 0, WORD bpp = 0);
	/// Destructor
	~fipImage();
	/**
	Image allocator
	@see FreeImage_AllocateT
	*/
	BOOL setSize(FREE_IMAGE_TYPE image_type, WORD width, WORD height, WORD bpp, unsigned red_mask = 0, unsigned green_mask = 0, unsigned blue_mask = 0);
	/// Destroy image data
	virtual void clear();
	//@}

	/**@name Copying */
	//@{
	/**
	Copy constructor
	@see FreeImage_Clone
	*/
	fipImage(const fipImage& src);
	/**
	Copy constructor
	@see FreeImage_Clone
	*/
	fipImage& operator=(const fipImage& src);
	/**
	<b>Assignement operator</b><br>
	Copy the input pointer and manage its destruction
	@see operator FIBITMAP*()
	*/
	fipImage& operator=(FIBITMAP *dib);


	/**
	@brief Copy a sub part of the current image and returns it as a fipImage object.

	This method works with any bitmap type.
	@param dst Output subimage
	@param left Specifies the left position of the cropped rectangle.
	@param top Specifies the top position of the cropped rectangle.
	@param right Specifies the right position of the cropped rectangle.
	@param bottom Specifies the bottom position of the cropped rectangle.
	@return Returns TRUE if successful, FALSE otherwise.
	@see FreeImage_Copy
	*/
	BOOL copySubImage(fipImage& dst, int left, int top, int right, int bottom) const;

	/**
	@brief Alpha blend or combine a sub part image with the current image.

    The bit depth of dst bitmap must be greater than or equal to the bit depth of src.
	Upper promotion of src is done internally. Supported bit depth equals to 4, 8, 16, 24 or 32.
	@param src Source subimage
	@param left Specifies the left position of the sub image.
	@param top Specifies the top position of the sub image.
	@param alpha Alpha blend factor. The source and destination images are alpha blended if
	alpha = 0..255. If alpha > 255, then the source image is combined to the destination image.
	@return Returns TRUE if successful, FALSE otherwise.
	@see FreeImage_Paste
	*/
	BOOL pasteSubImage(fipImage& src, int left, int top, int alpha = 256);

	/**
	@brief Crop a sub part of the current image and update it accordingly.

	This method works with any bitmap type.
	@param left Specifies the left position of the cropped rectangle.
	@param top Specifies the top position of the cropped rectangle.
	@param right Specifies the right position of the cropped rectangle.
	@param bottom Specifies the bottom position of the cropped rectangle.
	@return Returns TRUE if successful, FALSE otherwise.
	*/
	BOOL crop(int left, int top, int right, int bottom);


	//@}

	/** @name Loading & Saving
	 * Loading and saving is handled by the FreeImage library.
	 */
	//@{
	/**
	@brief Loads an image from disk, given its file name and an optional flag.
	@param lpszPathName Path and file name of the image to load.
	@param flag The signification of this flag depends on the image to be read.
	@return Returns TRUE if successful, FALSE otherwise.
	@see FreeImage_Load, FreeImage documentation
	*/
	BOOL load(const char* lpszPathName, int flag = 0);

	/**
	UNICODE version of load (this function only works under WIN32 and does nothing on other OS)
	@see load
	*/
	BOOL loadU(const wchar_t* lpszPathName, int flag = 0);

	/**
	@brief Loads an image using the specified FreeImageIO struct and fi_handle, and an optional flag.
	@param io FreeImageIO structure
	@param handle FreeImage fi_handle
	@param flag The signification of this flag depends on the image to be read.
	@return Returns TRUE if successful, FALSE otherwise.
	@see FreeImage_LoadFromHandle, FreeImage documentation
	*/
	BOOL loadFromHandle(FreeImageIO *io, fi_handle handle, int flag = 0);

	/**
	@brief Loads an image using the specified memory stream and an optional flag.
	@param memIO FreeImage memory stream
	@param flag The signification of this flag depends on the image to be read.
	@return Returns TRUE if successful, FALSE otherwise.
	@see FreeImage_LoadFromMemory, FreeImage documentation
	*/
	BOOL loadFromMemory(fipMemoryIO& memIO, int flag = 0);

	/**
	@brief Saves an image to disk, given its file name and an optional flag.
	@param lpszPathName Path and file name of the image to save.
	@param flag The signification of this flag depends on the image to be saved.
	@return Returns TRUE if successful, FALSE otherwise.
	@see FreeImage_Save, FreeImage documentation
	*/
	BOOL save(const char* lpszPathName, int flag = 0) const;

	/**
	UNICODE version of save (this function only works under WIN32 and does nothing on other OS)
	@see save
	*/
	BOOL saveU(const wchar_t* lpszPathName, int flag = 0) const;

	/**
	@brief Saves an image using the specified FreeImageIO struct and fi_handle, and an optional flag.
	@param fif Format identifier (FreeImage format)
	@param io FreeImageIO structure
	@param handle FreeImage fi_handle
	@param flag The signification of this flag depends on the image to be saved.
	@return Returns TRUE if successful, FALSE otherwise.
	@see FreeImage_SaveToHandle, FreeImage documentation
	*/
	BOOL saveToHandle(FREE_IMAGE_FORMAT fif, FreeImageIO *io, fi_handle handle, int flag = 0) const;

	/**
	@brief Saves an image using the specified memory stream and an optional flag.
	@param fif Format identifier (FreeImage format)
	@param memIO FreeImage memory stream
	@param flag The signification of this flag depends on the image to be saved.
	@return Returns TRUE if successful, FALSE otherwise.
	@see FreeImage_SaveToMemory, FreeImage documentation
	*/
	BOOL saveToMemory(FREE_IMAGE_FORMAT fif, fipMemoryIO& memIO, int flag = 0) const;

	//@}

	/**	@name Information functions
	 *  Accessors to the DIB BITMAPINFO structure.
	 */
	//@{

	/**
	Returns the data type of the image
	@see FreeImage_GetImageType
	*/
	FREE_IMAGE_TYPE getImageType() const;

	/**
	Returns the image width in pixels
	@see FreeImage_GetWidth
	*/
	WORD getWidth() const;

	/**
	Returns the image height in pixels
	@see FreeImage_GetHeight
	*/
	WORD getHeight() const;

	/**
	Returns the width of the bitmap in bytes rounded to the nearest DWORD.
	@see FreeImage_GetPitch
	*/
	WORD getScanWidth() const;

	/**
	Returns a pointer to the FIBITMAP data. Used for direct access from FREEIMAGE functions
	or from your own low level C functions.<br>
	<b>Sample use</b> : <br>
	<pre>
	fipImage src, dst;
	src.load("test.png");
	dst = FreeImage_ConvertTo8Bits(src);
	FreeImage_Save(FIF_TIFF, dst, "test.tif", 0);
	</pre>
	@see operator=(FIBITMAP *dib)
	*/
	operator FIBITMAP*() {
		return _dib;
	}

	/// Returns TRUE if the image is allocated, FALSE otherwise
	BOOL isValid() const;

	/**
	Returns a pointer to the bitmap's BITMAPINFO header.
	@see FreeImage_GetInfo
	*/
	BITMAPINFO* getInfo() const;

	/**
	Returns a pointer to the bitmap's BITMAPINFOHEADER.
	@see FreeImage_GetInfoHeader
	*/
    BITMAPINFOHEADER* getInfoHeader() const;

	/**
	Returns the size of the bitmap in bytes.
	The size of the bitmap is the BITMAPINFOHEADER + the size of the palette + the size of the bitmap data.
	@see FreeImage_GetDIBSize
	*/
	LONG getImageSize() const;

	/**
	Returns the bitdepth of the bitmap. <br>
	When the image type is FIT_BITMAP, valid bitdepth can be 1, 4, 8, 16, 24 or 32.
	@see FreeImage_GetBPP, getImageType
	*/
	WORD getBitsPerPixel() const;

	/**
	Returns the width of the bitmap in bytes.<br>
	<b>This is not the size of the scanline</b>.
	@see FreeImage_GetLine, getScanWidth
	*/
	WORD getLine() const;

	/**
	Returns the bitmap resolution along the X axis, in pixels / cm
	@see FreeImage_GetDotsPerMeterX
	*/
	double getHorizontalResolution() const;

	/**
	Returns the bitmap resolution along the Y axis, in pixels / cm
	@see FreeImage_GetDotsPerMeterY
	*/
	double getVerticalResolution() const;

	/**
	set the bitmap resolution along the X axis, in pixels / cm
	@see FreeImage_GetInfoHeader
	*/
	void setHorizontalResolution(double value);

	/**
	set the bitmap resolution along the Y axis, in pixels / cm
	@see FreeImage_GetInfoHeader
	*/
	void setVerticalResolution(double value);

	//@}

	/**@name Palette operations */
	//@{
	/**
	Returns a pointer to the bitmap's palette. If the bitmap doesn't have a palette, getPalette returns NULL.
	@see FreeImage_GetPalette
	*/
	RGBQUAD* getPalette() const;

	/**
	Returns the palette size in <b>bytes</b>.
	@see FreeImage_GetColorsUsed
	*/
	WORD getPaletteSize() const;

	/**
	Retrieves the number of colours used in the bitmap. If the bitmap is non-palletised, 0 is returned.
	@see FreeImage_GetColorsUsed
	*/
	WORD getColorsUsed() const;

	/**
	Investigates the colour type of the bitmap.
	@see FreeImage_GetColorType, FREE_IMAGE_COLOR_TYPE
	*/
	FREE_IMAGE_COLOR_TYPE getColorType() const;

	/**
	Returns TRUE if the bitmap is a 8-bit bitmap with a greyscale palette, FALSE otherwise
	@see FreeImage_GetBPP, FreeImage_GetColorType
	*/
	BOOL isGrayscale() const;
	//@}

	/**@name Pixel access */
	//@{

	/** @brief Returns a pointer to the bitmap bits.

	It is up to you to interpret these bytes correctly,
	according to the results of FreeImage_GetBPP and
	GetRedMask, FreeImage_GetGreenMask and FreeImage_GetBlueMask.<br>
	Use this function with getScanWidth to iterates through the pixels.
	@see FreeImage_GetBits
	*/
	BYTE* accessPixels() const;

	/** @brief Returns a pointer to the start of the given scanline in the bitmap’s data-bits.
		This pointer can be cast according to the result returned by getImageType.<br>
		Use this function with getScanWidth to iterates through the pixels.
		@see FreeImage_GetScanLine, FreeImage documentation
	*/
	BYTE* getScanLine(WORD scanline) const;

	/**
	Get the pixel index of a 1-, 4- or 8-bit palettized image at position (x, y), including range check (slow access).
	@param x Pixel position in horizontal direction
	@param y Pixel position in vertical direction
	@param value Pixel index (returned value)
	@return Returns TRUE if successfull, FALSE otherwise.
	@see FreeImage_GetPixelIndex
	*/
	BOOL getPixelIndex(unsigned x, unsigned y, BYTE *value) const;

	/**
	Get the pixel color of a 16-, 24- or 32-bit image at position (x, y), including range check (slow access).
	@param x Pixel position in horizontal direction
	@param y Pixel position in vertical direction
	@param value Pixel color (returned value)
	@return Returns TRUE if successfull, FALSE otherwise.
	@see FreeImage_GetPixelColor
	*/
	BOOL getPixelColor(unsigned x, unsigned y, RGBQUAD *value) const;

	/**
	Set the pixel index of a 1-, 4- or 8-bit palettized image at position (x, y), including range check (slow access).
	@param x Pixel position in horizontal direction
	@param y Pixel position in vertical direction
	@param value Pixel index
	@return Returns TRUE if successfull, FALSE otherwise.
	@see FreeImage_SetPixelIndex
	*/
	BOOL setPixelIndex(unsigned x, unsigned y, BYTE *value);

	/**
	Set the pixel color of a 16-, 24- or 32-bit image at position (x, y), including range check (slow access).
	@param x Pixel position in horizontal direction
	@param y Pixel position in vertical direction
	@param value Pixel color
	@return Returns TRUE if successfull, FALSE otherwise.
	@see FreeImage_SetPixelColor
	*/
	BOOL setPixelColor(unsigned x, unsigned y, RGBQUAD *value);

	//@}

	/**	@name Conversion routines
	 *  Bitmaps are always loaded in their default bit depth. If you want the bitmap to be stored in another bit depth, the class provides several conversion functions.
	 */
	//@{
	/**
	Converts an image to a type supported by FreeImage.
	@param image_type New image type
	@param scale_linear TRUE if image pixels must be scaled linearly when converting to a standard bitmap
	@return Returns TRUE if successfull, FALSE otherwise.
	@see FreeImage_ConvertToType, FreeImage_ConvertToStandardType
	*/
	BOOL convertToType(FREE_IMAGE_TYPE image_type, BOOL scale_linear = TRUE);

	/**
	Converts the bitmap to 1 bit using a threshold T.
	@param T Threshold value in [0..255]
	@return Returns TRUE if successfull, FALSE otherwise.
	@see FreeImage_Threshold
	*/
	BOOL threshold(BYTE T);

	/**
	Converts a 8-bit image to a monochrome 1-bit image using a dithering algorithm.
	@param algorithm Dithering algorithm to use.
	@return Returns TRUE if successfull, FALSE otherwise.
	@see FreeImage_Dither, FREE_IMAGE_DITHER
	*/
	BOOL dither(FREE_IMAGE_DITHER algorithm);

	/**
	Converts the bitmap to 4 bits. Unless the bitmap is a 1-bit palettized bitmap, colour values are converted to greyscale.
	@return Returns TRUE if successfull, FALSE otherwise.
	@see FreeImage_ConvertTo4Bits
	*/
	BOOL convertTo4Bits();

	/**
	Converts the bitmap to 8 bits. If the bitmap is 24 or 32-bit RGB, the colour values are converted to greyscale.
	@return Returns TRUE if successfull, FALSE otherwise.
	@see FreeImage_ConvertTo8Bits
	*/
	BOOL convertTo8Bits();

	/**
	Converts the bitmap to 8 bits.<br>
	For palletized bitmaps, the color map is converted to a greyscale ramp.
	@see FreeImage_ConvertToGreyscale
	@return Returns TRUE if successfull, FALSE otherwise.
	*/
	BOOL convertToGrayscale();

	/**
	Quantizes a full colour 24-bit bitmap to a palletised 8-bit bitmap.<br>
	The quantize parameter specifies which colour reduction algorithm should be used.
	@param algorithm Color quantization algorithm to use.
	@return Returns TRUE if successfull, FALSE otherwise.
	@see FreeImage_ColorQuantize, FREE_IMAGE_QUANTIZE
	*/
	BOOL colorQuantize(FREE_IMAGE_QUANTIZE algorithm);

	/**
	Converts the bitmap to 16 bits. The resulting bitmap has a layout of 5 bits red, 5 bits green, 5 bits blue and 1 unused bit.
	@return Returns TRUE if successfull, FALSE otherwise.
	@see FreeImage_ConvertTo16Bits555
	*/
	BOOL convertTo16Bits555();

	/**
	Converts the bitmap to 16 bits. The resulting bitmap has a layout of 5 bits red, 6 bits green and 5 bits blue.
	@return Returns TRUE if successfull, FALSE otherwise.
	@see FreeImage_ConvertTo16Bits565
	*/
	BOOL convertTo16Bits565();

	/**
	Converts the bitmap to 24 bits.
	@return Returns TRUE if successfull, FALSE otherwise.
	@see FreeImage_ConvertTo24Bits
	*/
	BOOL convertTo24Bits();

	/**
	Converts the bitmap to 32 bits.
	@return Returns TRUE if successfull, FALSE otherwise.
	@see FreeImage_ConvertTo32Bits
	*/
	BOOL convertTo32Bits();

	/**
	Converts the bitmap to a 96-bit RGBF image.
	@return Returns TRUE if successfull, FALSE otherwise.
	@see FreeImage_ConvertToRGBF
	*/
	BOOL convertToRGBF();

	/**
	Converts a High Dynamic Range image (48-bit RGB or 96-bit RGB Float) to a 24-bit RGB image.
	@param tmo Tone mapping operator
	@param first_param First tone mapping algorithm parameter (algorithm dependant)
	@param second_param Second tone mapping algorithm parameter (algorithm dependant)
	@return Returns TRUE if successfull, FALSE otherwise.
	@see FreeImage_ToneMapping
	*/
	BOOL toneMapping(FREE_IMAGE_TMO tmo, double first_param = 0, double second_param = 0);

	//@}

	/**	@name Transparency support: background colour and alpha channel */
	//@{

	/**
	Returns TRUE if the image is transparent, returns FALSE otherwise
	@see FreeImage_IsTransparent
	*/
	BOOL isTransparent() const;

	/**
	8-bit transparency : get the number of transparent colors.
	@return Returns the number of transparent colors in a palletised bitmap.
	@see FreeImage_GetTransparencyCount
	*/
	unsigned getTransparencyCount() const;

	/**
	8-bit transparency : get the bitmap’s transparency table.
	@return Returns a pointer to the bitmap’s transparency table.
	@see FreeImage_GetTransparencyTable
	*/
	BYTE* getTransparencyTable() const;

	/**
	8-bit transparency : set the bitmap’s transparency table.
	@see FreeImage_SetTransparencyTable
	*/
	void setTransparencyTable(BYTE *table, int count);

	/**
	Returns TRUE when the image has a file background color, FALSE otherwise.
	@see FreeImage_HasBackgroundColor
	*/
	BOOL hasFileBkColor() const;

	/**
	@brief Retrieves the file background color of an image.

	For 8-bit images, the color index
	in the palette is returned in the rgbReserved member of the bkcolor parameter.
	@return Returns TRUE if successful, FALSE otherwise.
	@see FreeImage_GetBackgroundColor
	*/
	BOOL getFileBkColor(RGBQUAD *bkcolor) const;

	/**
	@brief Set the file background color of an image.

	When saving an image to PNG, this background color is transparently saved to the PNG file.
	When the bkcolor parameter is NULL, the background color is removed from the image.
	@return Returns TRUE if successful, FALSE otherwise.
	@see FreeImage_SetBackgroundColor
	*/
	BOOL setFileBkColor(RGBQUAD *bkcolor);
	//@}

	/**@name Channel processing support */
	//@{
	/** @brief Retrieves the red, green, blue or alpha channel of a 24- or 32-bit BGR[A] image.
	@param image Output image to be extracted
	@param channel Color channel to extract
	@return Returns TRUE if successful, FALSE otherwise.
	@see FreeImage_GetChannel, FREE_IMAGE_COLOR_CHANNEL
	*/
	BOOL getChannel(fipImage& image, FREE_IMAGE_COLOR_CHANNEL channel) const;

	/**
	@brief Insert a 8-bit dib into a 24- or 32-bit image.
	@param image Input 8-bit image to insert
	@param channel Color channel to replace
	@return Returns TRUE if successful, FALSE otherwise.
	@see FreeImage_SetChannel, FREE_IMAGE_COLOR_CHANNEL
	*/
	BOOL setChannel(fipImage& image, FREE_IMAGE_COLOR_CHANNEL channel);

	/** @brief Split a 24-bit RGB image into 3 greyscale images corresponding to the red, green and blue channels.
	@param RedChannel Output red channel.
	@param GreenChannel Output green channel.
	@param BlueChannel Output blue channel.
	@return Returns FALSE if the dib isn't a valid image, if it's not a 24-bit image or if
	one of the output channel can't be allocated. Returns TRUE otherwise.
	@see FreeImage_GetChannel
	*/
	BOOL splitChannels(fipImage& RedChannel, fipImage& GreenChannel, fipImage& BlueChannel);

	/** @brief Builds a 24-bit RGB image given its red, green and blue channel.
	@param red Input red channel.
	@param green Input green channel.
	@param blue Input blue channel.
	@return Returns FALSE if the dib can't be allocated, if the input channels are not 8-bit images. Returns TRUE otherwise.
	@see FreeImage_SetChannel
	*/
	BOOL combineChannels(fipImage& red, fipImage& green, fipImage& blue);
	//@}

	/**@name Rotation and flipping */
	//@{
	/**
	Image translation and rotation using B-Splines.
	@param angle Image rotation angle, in degree
	@param x_shift Image horizontal shift
	@param y_shift Image vertical shift
	@param x_origin Origin of the x-axis
	@param y_origin Origin of the y-axis
	@param use_mask Whether or not to mask the image. Image mirroring is applied when use_mask is set to FALSE
	@return Returns the translated & rotated dib if successful, returns NULL otherwise
	@see FreeImage_RotateEx
	*/
	BOOL rotateEx(double angle, double x_shift, double y_shift, double x_origin, double y_origin, BOOL use_mask);

	/**
	Image rotation by means of three shears.
	@param angle Image rotation angle, in degree
	@return Returns rotated dib if successful, returns NULL otherwise
	@see FreeImage_RotateClassic
	*/
	BOOL rotate(double angle);

	/**
	Flip the image horizontally along the vertical axis
	@see FreeImage_FlipHorizontal
	*/
	BOOL flipHorizontal();

	/**
	Flip the image vertically along the horizontal axis
	@see FreeImage_FlipVertical
	*/
	BOOL flipVertical();
	//@}

	/**@name Color manipulation routines */
	//@{
	/**
	Inverts each pixel data.
	@return Returns TRUE if successful, FALSE otherwise.
	@see FreeImage_Invert
	*/
	BOOL invert();

	/** @brief Perfoms an histogram transformation on a 8, 24 or 32-bit image
	according to the values of a lookup table (LUT).

	The transformation is done as follows.<br>
	Image 8-bit : if the image has a color palette, the LUT is applied to this palette,
	otherwise, it is applied to the grey values.<br>
	Image 24-bit & 32-bit : if channel == IPL_CC_RGB, the same LUT is applied to each color
	plane (R,G, and B). Otherwise, the LUT is applied to the specified channel only.
	@param LUT Lookup table. <b>The size of 'LUT' is assumed to be 256.</b>
	@param channel The color channel to be processed (only used with 24 & 32-bit DIB).
	@return Returns TRUE if the operation was successful, FALSE otherwise
	@see FreeImage_AdjustCurve, FREE_IMAGE_COLOR_CHANNEL
	*/
	BOOL adjustCurve(BYTE *LUT, FREE_IMAGE_COLOR_CHANNEL channel);

	/** @brief Performs gamma correction on a 8, 24 or 32-bit image.
	@param gamma Gamma value to use. A value of 1.0 leaves the image alone,
	less than one darkens it, and greater than one lightens it.
	@return Returns TRUE if the operation was successful, FALSE otherwise
	@see FreeImage_AdjustGamma, adjustCurve
	*/
	BOOL adjustGamma(double gamma);

	/** @brief Adjusts the brightness of a 8, 24 or 32-bit image by a certain amount.
	@param percentage Where -100 <= percentage <= 100<br>
	A value 0 means no change, less than 0 will make the image darker
	and greater than 0 will make the image brighter.
	@return Returns TRUE if the operation was succesful, FALSE otherwise
	@see FreeImage_AdjustBrightness, adjustCurve
	*/
	BOOL adjustBrightness(double percentage);

	/** @brief Adjusts the contrast of a 8, 24 or 32-bit image by a certain amount.
	@param percentage Where -100 <= percentage <= 100<br>
	A value 0 means no change, less than 0 will decrease the contrast
	and greater than 0 will increase the contrast of the image.
	@return Returns TRUE if the operation was succesfull, FALSE otherwise
	@see FreeImage_AdjustContrast, adjustCurve
	*/
	BOOL adjustContrast(double percentage);

	/** @brief Computes image histogram

	For 24-bit and 32-bit images, histogram can be computed from red, green, blue and
	black channels. For 8-bit images, histogram is computed from the black channel. Other
	bit depth is not supported.
	@param histo pointer to an histogram array. <b>Size of this array is assumed to be 256</b>.
	@param channel Color channel to use
	@return Returns TRUE if the operation was succesfull, FALSE otherwise
	@see FreeImage_GetHistogram
	*/
	BOOL getHistogram(DWORD *histo, FREE_IMAGE_COLOR_CHANNEL channel = FICC_BLACK) const;
	//@}

	/**@name Upsampling / downsampling */
	//@{

	/** @brief Rescale the image to a new width / height.

	@param new_width New image width
	@param new_height New image height
	@param filter The filter parameter specifies which resampling filter should be used.
	@return Returns TRUE if the operation was successful, FALSE otherwise
	@see FreeImage_Rescale, FREE_IMAGE_FILTER
	*/
	BOOL rescale(WORD new_width, WORD new_height, FREE_IMAGE_FILTER filter);

	/** @brief Creates a thumbnail image keeping aspect ratio

	@param max_size Maximum width or height in pixel units
	@param convert When set to TRUE, converts the image to a standard type
	@return Returns TRUE if the operation was successful, FALSE otherwise
	@see FreeImage_MakeThumbnail
	*/
	BOOL makeThumbnail(WORD max_size, BOOL convert = TRUE);
	//@}

	/**@name Image status */
	//@{
	/**
	Set the image status as 'modified'.<br>
	When using the fipWinImage class, the image status is used to refresh the display.
	It is changed to FALSE whenever the display has just been refreshed.
	@param bStatus TRUE if the image should be marked as modified, FALSE otherwise
	@see isModified
	*/
	void setModified(BOOL bStatus = TRUE) {
		_bHasChanged = bStatus;
	}

	/**
	Get the image status
	@return Returns TRUE if the image is marked as modified, FALSE otherwise
	@see setModified
	*/
	BOOL isModified() {
		return _bHasChanged;
	}
	//@}

	/**@name Metadata */
	//@{
	/**
	Returns the number of tags contained in the <i>model</i> metadata model
	attached to the dib
	@param model Metadata model to look for
	*/
	/**
	Retrieve a metadata attached to the dib
	@param model Metadata model to look for
	@param key Metadata field name
	@param tag Returned tag
	@return Returns TRUE if the operation was succesfull, FALSE otherwise
	@see FreeImage_GetMetadata
	*/
//	BOOL getMetadata(FREE_IMAGE_MDMODEL model, const char *key, fipTag& tag) const;
	/**
	Attach a new FreeImage tag to the dib.<br>
	<b>Sample use</b> : <br>
	<pre>
	fipImage image;
	// ...
	fipTag tag;
	tag.setKeyValue("Caption/Abstract", "my caption");
	image.setMetadata(FIMD_IPTC, tag.getKey(), tag);
	tag.setKeyValue("Keywords", "FreeImage;Library;Images;Compression");
	image.setMetadata(FIMD_IPTC, tag.getKey(), tag);
	</pre>

	@param model Metadata model used to store the tag
	@param key Tag field name
	@param tag Tag to be attached
	@return Returns TRUE if the operation was succesfull, FALSE otherwise
	@see FreeImage_SetMetadata
	*/
//	BOOL setMetadata(FREE_IMAGE_MDMODEL model, const char *key, fipTag& tag);
	//@}


  protected:
	/**@name Internal use */
	//@{
	  BOOL replace(FIBITMAP *new_dib);
	//@}
};

//////////////////////////////////////////////////////////////////////

class fipMemoryIO : public fipObject
{
protected:
	/// Pointer to a memory stream
	FIMEMORY *_hmem;

public :
	/** Constructor.
	Wrap a memory buffer containing image data.<br>
	The memory buffer is read only and has to be freed by the user
	when no longer in use.<br>
	When default arguments are used, open a memory file as read/write.
	@param data Pointer to the memory buffer
	@param size_in_bytes Buffer size in bytes
	@see FreeImage_OpenMemory
	*/
    fipMemoryIO(BYTE *data = NULL, DWORD size_in_bytes = 0);

	/** Destructor.
	Free any allocated memory
	@see FreeImage_CloseMemory
	*/
	~fipMemoryIO();

	/** Returns TRUE if the internal memory buffer is a valid buffer, returns FALSE otherwise
	*/
	BOOL isValid() const;

	/** Returns the buffer image format
	@see FreeImage_GetFileTypeFromMemory
	*/
	FREE_IMAGE_FORMAT getFileType() const;

	/**
	Returns a pointer to the FIMEMORY data. Used for direct access from FREEIMAGE functions
	or from your own low level C functions.
	*/
	operator FIMEMORY*() {
		return _hmem;
	}

	/**@name Memory IO routines */
	//@{
	/**
	Loads a dib from a memory stream
	@param fif Format identifier (FreeImage format)
	@param flags The signification of this flag depends on the image to be loaded.
	@return Returns the loaded dib if successful, returns NULL otherwise
	@see FreeImage_LoadFromMemory
	*/
	FIBITMAP* load(FREE_IMAGE_FORMAT fif, int flags = 0) const;
	/**
	Saves a dib to a memory stream
	@param fif Format identifier (FreeImage format)
	@param dib Image to be saved
	@param flags The signification of this flag depends on the image to be saved.
	@return Returns TRUE if successful, returns FALSE otherwise
	@see FreeImage_SaveToMemory
	*/
	BOOL save(FREE_IMAGE_FORMAT fif, FIBITMAP *dib, int flags = 0);
	/**
	Reads data from a memory stream
	@param buffer Storage location for data
	@param size Item size in bytes
	@param count Maximum number of items to be read
	@return Returns the number of full items actually read, which may be less than count if an error occurs
	@see FreeImage_ReadMemory
	*/
	unsigned read(void *buffer, unsigned size, unsigned count) const;
	/**
	Writes data to a memory stream
	@param buffer Pointer to data to be written
	@param size Item size in bytes
	@param count Maximum number of items to be written
	@return Returns the number of full items actually written, which may be less than count if an error occurs
	@see FreeImage_WriteMemory
	*/
	unsigned write(const void *buffer, unsigned size, unsigned count);
	/**
	Gets the current position of a memory pointer
	@see FreeImage_TellMemory
	*/
	long tell() const;
	/**
	Moves the memory pointer to a specified location
	@see FreeImage_SeekMemory
	*/
	BOOL seek(long offset, int origin);
	/**
	Provides a direct buffer access to a memory stream
	@param data Pointer to the memory buffer (returned value)
	@param size_in_bytes Buffer size in bytes (returned value)
	@see FreeImage_AcquireMemory
	*/
	BOOL acquire(BYTE **data, DWORD *size_in_bytes);
	//@}

private:
	/// Disable copy
	fipMemoryIO(const fipMemoryIO& src);
	/// Disable copy
	fipMemoryIO& operator=(const fipMemoryIO& src);

};

BOOL fipImage::replace(FIBITMAP *new_dib) {
	if(new_dib == NULL)
		return FALSE;
	if(_dib)
		FreeImage_Unload(_dib);
	_dib = new_dib;
	_bHasChanged = TRUE;
	return TRUE;
}

///////////////////////////////////////////////////////////////////
// Creation & Destruction

fipImage::fipImage(FREE_IMAGE_TYPE image_type, WORD width, WORD height, WORD bpp) {
	_dib = NULL;
	_bHasChanged = FALSE;
	if(width && height && bpp)
		setSize(image_type, width, height, bpp);
}

fipImage::~fipImage() {
	if(_dib) {
		FreeImage_Unload(_dib);
		_dib = NULL;
	}
}

BOOL fipImage::setSize(FREE_IMAGE_TYPE image_type, WORD width, WORD height, WORD bpp, unsigned red_mask, unsigned green_mask, unsigned blue_mask) {
	if(_dib) {
		FreeImage_Unload(_dib);
	}
	if((_dib = FreeImage_AllocateT(image_type, width, height, bpp, red_mask, green_mask, blue_mask)) == NULL)
		return FALSE;

	if(image_type == FIT_BITMAP) {
		// Create palette if needed
		switch(bpp)	{
			case 1:
			case 4:
			case 8:
				RGBQUAD *pal = FreeImage_GetPalette(_dib);
				for(unsigned i = 0; i < FreeImage_GetColorsUsed(_dib); i++) {
					pal[i].rgbRed = i;
					pal[i].rgbGreen = i;
					pal[i].rgbBlue = i;
				}
				break;
		}
	}

	_bHasChanged = TRUE;

	return TRUE;
}

void fipImage::clear() {
	if(_dib) {
		FreeImage_Unload(_dib);
		_dib = NULL;
	}
	_bHasChanged = TRUE;
}

///////////////////////////////////////////////////////////////////
// Copying

fipImage::fipImage(const fipImage& Image) : fipObject(Image)
{
	_dib = NULL;
	FIBITMAP *clone = FreeImage_Clone((FIBITMAP*)Image._dib);
	replace(clone);
}

fipImage& fipImage::operator=(const fipImage& Image) {
	if(this != &Image) {
		FIBITMAP *clone = FreeImage_Clone((FIBITMAP*)Image._dib);
		replace(clone);
	}
	return *this;
}

fipImage& fipImage::operator=(FIBITMAP *dib) {
	replace(dib);
	return *this;
}

BOOL fipImage::copySubImage(fipImage& dst, int left, int top, int right, int bottom) const {
	if(_dib) {
		dst = FreeImage_Copy(_dib, left, top, right, bottom);
		return dst.isValid();
	}
	return FALSE;
}

BOOL fipImage::pasteSubImage(fipImage& src, int left, int top, int alpha) {
	if(_dib) {
		BOOL bResult = FreeImage_Paste(_dib, src._dib, left, top, alpha);
		_bHasChanged = TRUE;
		return bResult;
	}
	return FALSE;
}

BOOL fipImage::crop(int left, int top, int right, int bottom) {
	if(_dib) {
		FIBITMAP *dst = FreeImage_Copy(_dib, left, top, right, bottom);
		return replace(dst);
	}
	return FALSE;
}


///////////////////////////////////////////////////////////////////
// Information functions

FREE_IMAGE_TYPE fipImage::getImageType() const {
	return FreeImage_GetImageType(_dib);
}

WORD fipImage::getWidth() const {
	return FreeImage_GetWidth(_dib);
}

WORD fipImage::getHeight() const {
	return FreeImage_GetHeight(_dib);
}

WORD fipImage::getScanWidth() const {
	return FreeImage_GetPitch(_dib);
}

BOOL fipImage::isValid() const {
	return (_dib != NULL) ? TRUE:FALSE;
}

BITMAPINFO* fipImage::getInfo() const {
	return FreeImage_GetInfo(_dib);
}

BITMAPINFOHEADER* fipImage::getInfoHeader() const {
	return FreeImage_GetInfoHeader(_dib);
}

LONG fipImage::getImageSize() const {
	return FreeImage_GetDIBSize(_dib);
}

WORD fipImage::getBitsPerPixel() const {
	return FreeImage_GetBPP(_dib);
}

WORD fipImage::getLine() const {
	return FreeImage_GetLine(_dib);
}

double fipImage::getHorizontalResolution() const {
	return (FreeImage_GetDotsPerMeterX(_dib) / (double)100);
}

double fipImage::getVerticalResolution() const {
	return (FreeImage_GetDotsPerMeterY(_dib) / (double)100);
}

void fipImage::setHorizontalResolution(double value) {
	FreeImage_SetDotsPerMeterX(_dib, (unsigned)(value * 100 + 0.5));
}

void fipImage::setVerticalResolution(double value) {
	FreeImage_SetDotsPerMeterY(_dib, (unsigned)(value * 100 + 0.5));
}


///////////////////////////////////////////////////////////////////
// Palette operations

RGBQUAD* fipImage::getPalette() const {
	return FreeImage_GetPalette(_dib);
}

WORD fipImage::getPaletteSize() const {
	return FreeImage_GetColorsUsed(_dib) * sizeof(RGBQUAD);
}

WORD fipImage::getColorsUsed() const {
	return FreeImage_GetColorsUsed(_dib);
}

FREE_IMAGE_COLOR_TYPE fipImage::getColorType() const {
	return FreeImage_GetColorType(_dib);
}

BOOL fipImage::isGrayscale() const {
	return ((FreeImage_GetBPP(_dib) == 8) && (FreeImage_GetColorType(_dib) != FIC_PALETTE));
}

///////////////////////////////////////////////////////////////////
// Pixel access

BYTE* fipImage::accessPixels() const {
	return FreeImage_GetBits(_dib);
}

BYTE* fipImage::getScanLine(WORD scanline) const {
	if(scanline < FreeImage_GetHeight(_dib)) {
		return FreeImage_GetScanLine(_dib, scanline);
	}
	return NULL;
}

BOOL fipImage::getPixelIndex(unsigned x, unsigned y, BYTE *value) const {
	return FreeImage_GetPixelIndex(_dib, x, y, value);
}

BOOL fipImage::getPixelColor(unsigned x, unsigned y, RGBQUAD *value) const {
	return FreeImage_GetPixelColor(_dib, x, y, value);
}

BOOL fipImage::setPixelIndex(unsigned x, unsigned y, BYTE *value) {
	_bHasChanged = TRUE;
	return FreeImage_SetPixelIndex(_dib, x, y, value);
}

BOOL fipImage::setPixelColor(unsigned x, unsigned y, RGBQUAD *value) {
	_bHasChanged = TRUE;
	return FreeImage_SetPixelColor(_dib, x, y, value);
}

///////////////////////////////////////////////////////////////////
// Loading & Saving

BOOL fipImage::load(const char* lpszPathName, int flag) {
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	// check the file signature and get its format
	// (the second argument is currently not used by FreeImage)
	fif = FreeImage_GetFileType(lpszPathName, 0);
	if(fif == FIF_UNKNOWN) {
		// no signature ?
		// try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilename(lpszPathName);
	}
	// check that the plugin has reading capabilities ...
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
		// Free the previous dib
		if(_dib) {
			FreeImage_Unload(_dib);
		}
		// Load the file
		_dib = FreeImage_Load(fif, lpszPathName, flag);
		_bHasChanged = TRUE;
		if(_dib == NULL)
			return FALSE;
		return TRUE;
	}
	return FALSE;
}

BOOL fipImage::loadU(const wchar_t* lpszPathName, int flag) {
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	// check the file signature and get its format
	// (the second argument is currently not used by FreeImage)
	fif = FreeImage_GetFileTypeU(lpszPathName, 0);
	if(fif == FIF_UNKNOWN) {
		// no signature ?
		// try to guess the file format from the file extension
		fif = FreeImage_GetFIFFromFilenameU(lpszPathName);
	}
	// check that the plugin has reading capabilities ...
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
		// Free the previous dib
		if(_dib) {
			FreeImage_Unload(_dib);
		}
		// Load the file
		_dib = FreeImage_LoadU(fif, lpszPathName, flag);
		_bHasChanged = TRUE;
		if(_dib == NULL)
			return FALSE;
		return TRUE;
	}
	return FALSE;
}

BOOL fipImage::loadFromHandle(FreeImageIO *io, fi_handle handle, int flag) {
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	// check the file signature and get its format
	fif = FreeImage_GetFileTypeFromHandle(io, handle, 16);
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
		// Free the previous dib
		if(_dib) {
			FreeImage_Unload(_dib);
		}
		// Load the file
		_dib = FreeImage_LoadFromHandle(fif, io, handle, flag);
		_bHasChanged = TRUE;
		if(_dib == NULL)
			return FALSE;
		return TRUE;
	}
	return FALSE;
}

BOOL fipImage::loadFromMemory(fipMemoryIO& memIO, int flag) {
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	// check the file signature and get its format
	fif = memIO.getFileType();
	if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
		// Free the previous dib
		if(_dib) {
			FreeImage_Unload(_dib);
		}
		// Load the file
		_dib = memIO.load(fif, flag);
		_bHasChanged = TRUE;
		if(_dib == NULL)
			return FALSE;
		return TRUE;
	}
	return FALSE;
}

BOOL fipImage::save(const char* lpszPathName, int flag) const {
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	BOOL bSuccess = FALSE;

	// Try to guess the file format from the file extension
	fif = FreeImage_GetFIFFromFilename(lpszPathName);
	if(fif != FIF_UNKNOWN ) {
		// Check that the dib can be saved in this format
		BOOL bCanSave;

		FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(_dib);
		if(image_type == FIT_BITMAP) {
			// standard bitmap type
			WORD bpp = FreeImage_GetBPP(_dib);
			bCanSave = (FreeImage_FIFSupportsWriting(fif) && FreeImage_FIFSupportsExportBPP(fif, bpp));
		} else {
			// special bitmap type
			bCanSave = FreeImage_FIFSupportsExportType(fif, image_type);
		}

		if(bCanSave) {
			bSuccess = FreeImage_Save(fif, _dib, lpszPathName, flag);
			return bSuccess;
		}
	}
	return bSuccess;
}

BOOL fipImage::saveU(const wchar_t* lpszPathName, int flag) const {
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	BOOL bSuccess = FALSE;

	// Try to guess the file format from the file extension
	fif = FreeImage_GetFIFFromFilenameU(lpszPathName);
	if(fif != FIF_UNKNOWN ) {
		// Check that the dib can be saved in this format
		BOOL bCanSave;

		FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(_dib);
		if(image_type == FIT_BITMAP) {
			// standard bitmap type
			WORD bpp = FreeImage_GetBPP(_dib);
			bCanSave = (FreeImage_FIFSupportsWriting(fif) && FreeImage_FIFSupportsExportBPP(fif, bpp));
		} else {
			// special bitmap type
			bCanSave = FreeImage_FIFSupportsExportType(fif, image_type);
		}

		if(bCanSave) {
			bSuccess = FreeImage_SaveU(fif, _dib, lpszPathName, flag);
			return bSuccess;
		}
	}
	return bSuccess;
}

BOOL fipImage::saveToHandle(FREE_IMAGE_FORMAT fif, FreeImageIO *io, fi_handle handle, int flag) const {
	BOOL bSuccess = FALSE;

	if(fif != FIF_UNKNOWN ) {
		// Check that the dib can be saved in this format
		BOOL bCanSave;

		FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(_dib);
		if(image_type == FIT_BITMAP) {
			// standard bitmap type
			WORD bpp = FreeImage_GetBPP(_dib);
			bCanSave = (FreeImage_FIFSupportsWriting(fif) && FreeImage_FIFSupportsExportBPP(fif, bpp));
		} else {
			// special bitmap type
			bCanSave = FreeImage_FIFSupportsExportType(fif, image_type);
		}

		if(bCanSave) {
			bSuccess = FreeImage_SaveToHandle(fif, _dib, io, handle, flag);
			return bSuccess;
		}
	}
	return bSuccess;
}

BOOL fipImage::saveToMemory(FREE_IMAGE_FORMAT fif, fipMemoryIO& memIO, int flag) const {
	BOOL bSuccess = FALSE;

	if(fif != FIF_UNKNOWN ) {
		// Check that the dib can be saved in this format
		BOOL bCanSave;

		FREE_IMAGE_TYPE image_type = FreeImage_GetImageType(_dib);
		if(image_type == FIT_BITMAP) {
			// standard bitmap type
			WORD bpp = FreeImage_GetBPP(_dib);
			bCanSave = (FreeImage_FIFSupportsWriting(fif) && FreeImage_FIFSupportsExportBPP(fif, bpp));
		} else {
			// special bitmap type
			bCanSave = FreeImage_FIFSupportsExportType(fif, image_type);
		}

		if(bCanSave) {
			bSuccess = memIO.save(fif, _dib, flag);
			return bSuccess;
		}
	}
	return bSuccess;
}

///////////////////////////////////////////////////////////////////
// Conversion routines

BOOL fipImage::convertToType(FREE_IMAGE_TYPE image_type, BOOL scale_linear) {
	if(_dib) {
		FIBITMAP *dib = FreeImage_ConvertToType(_dib, image_type, scale_linear);
		return replace(dib);
	}
	return FALSE;
}

BOOL fipImage::threshold(BYTE T) {
	if(_dib) {
		FIBITMAP *dib1 = FreeImage_Threshold(_dib, T);
		return replace(dib1);
	}
	return FALSE;
}

BOOL fipImage::convertTo4Bits() {
	if(_dib) {
		FIBITMAP *dib4 = FreeImage_ConvertTo4Bits(_dib);
		return replace(dib4);
	}
	return FALSE;
}

BOOL fipImage::convertTo8Bits() {
	if(_dib) {
		FIBITMAP *dib8 = FreeImage_ConvertTo8Bits(_dib);
		return replace(dib8);
	}
	return FALSE;
}

BOOL fipImage::convertTo16Bits555() {
	if(_dib) {
		FIBITMAP *dib16_555 = FreeImage_ConvertTo16Bits555(_dib);
		return replace(dib16_555);
	}
	return FALSE;
}

BOOL fipImage::convertTo16Bits565() {
	if(_dib) {
		FIBITMAP *dib16_565 = FreeImage_ConvertTo16Bits565(_dib);
		return replace(dib16_565);
	}
	return FALSE;
}

BOOL fipImage::convertTo24Bits() {
	if(_dib) {
		FIBITMAP *dibRGB = FreeImage_ConvertTo24Bits(_dib);
		return replace(dibRGB);
	}
	return FALSE;
}

BOOL fipImage::convertTo32Bits() {
	if(_dib) {
		FIBITMAP *dib32 = FreeImage_ConvertTo32Bits(_dib);
		return replace(dib32);
	}
	return FALSE;
}

BOOL fipImage::convertToGrayscale() {
	if(_dib) {
		FIBITMAP *dib8 = FreeImage_ConvertToGreyscale(_dib);
		return replace(dib8);
	}
	return FALSE;
}

BOOL fipImage::colorQuantize(FREE_IMAGE_QUANTIZE algorithm) {
	if(_dib) {
		FIBITMAP *dib8 = FreeImage_ColorQuantize(_dib, algorithm);
		return replace(dib8);
	}
	return FALSE;
}

BOOL fipImage::dither(FREE_IMAGE_DITHER algorithm) {
	if(_dib) {
		FIBITMAP *dib = FreeImage_Dither(_dib, algorithm);
		return replace(dib);
	}
	return FALSE;
}

BOOL fipImage::convertToRGBF() {
	if(_dib) {
		FIBITMAP *dib = FreeImage_ConvertToRGBF(_dib);
		return replace(dib);
	}
	return FALSE;

}


BOOL fipImage::toneMapping(FREE_IMAGE_TMO tmo, double first_param, double second_param) {
	if(_dib) {
		FIBITMAP *dib = FreeImage_ToneMapping(_dib, tmo, first_param, second_param);
		return replace(dib);
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////
// Transparency support: background colour and alpha channel

BOOL fipImage::isTransparent() const {
	return FreeImage_IsTransparent(_dib);
}

unsigned fipImage::getTransparencyCount() const {
	return FreeImage_GetTransparencyCount(_dib);
}

BYTE* fipImage::getTransparencyTable() const {
	return FreeImage_GetTransparencyTable(_dib);
}

void fipImage::setTransparencyTable(BYTE *table, int count) {
	FreeImage_SetTransparencyTable(_dib, table, count);
	_bHasChanged = TRUE;
}

BOOL fipImage::hasFileBkColor() const {
	return FreeImage_HasBackgroundColor(_dib);
}

BOOL fipImage::getFileBkColor(RGBQUAD *bkcolor) const {
	return FreeImage_GetBackgroundColor(_dib, bkcolor);
}

BOOL fipImage::setFileBkColor(RGBQUAD *bkcolor) {
	_bHasChanged = TRUE;
	return FreeImage_SetBackgroundColor(_dib, bkcolor);
}

///////////////////////////////////////////////////////////////////
// Channel processing support

BOOL fipImage::getChannel(fipImage& image, FREE_IMAGE_COLOR_CHANNEL channel) const {
	if(_dib) {
		image = FreeImage_GetChannel(_dib, channel);
		return image.isValid();
	}
	return FALSE;
}

BOOL fipImage::setChannel(fipImage& image, FREE_IMAGE_COLOR_CHANNEL channel) {
	if(_dib) {
		_bHasChanged = TRUE;
		return FreeImage_SetChannel(_dib, image._dib, channel);
	}
	return FALSE;
}

BOOL fipImage::splitChannels(fipImage& RedChannel, fipImage& GreenChannel, fipImage& BlueChannel) {
	if(_dib) {
		RedChannel = FreeImage_GetChannel(_dib, FICC_RED);
		GreenChannel = FreeImage_GetChannel(_dib, FICC_GREEN);
		BlueChannel = FreeImage_GetChannel(_dib, FICC_BLUE);

		return (RedChannel.isValid() && GreenChannel.isValid() && BlueChannel.isValid());
	}
	return FALSE;
}

BOOL fipImage::combineChannels(fipImage& red, fipImage& green, fipImage& blue) {
	if(!_dib) {
		int width = red.getWidth();
		int height = red.getHeight();
		_dib = FreeImage_Allocate(width, height, 24, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK);
	}

	if(_dib) {
		BOOL bResult = TRUE;
		bResult &= FreeImage_SetChannel(_dib, red._dib, FICC_RED);
		bResult &= FreeImage_SetChannel(_dib, green._dib, FICC_GREEN);
		bResult &= FreeImage_SetChannel(_dib, blue._dib, FICC_BLUE);

		_bHasChanged = TRUE;

		return bResult;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////
// Rotation and flipping

BOOL fipImage::rotateEx(double angle, double x_shift, double y_shift, double x_origin, double y_origin, BOOL use_mask) {
	if(_dib) {
		if(FreeImage_GetBPP(_dib) >= 8) {
			FIBITMAP *rotated = FreeImage_RotateEx(_dib, angle, x_shift, y_shift, x_origin, y_origin, use_mask);
			return replace(rotated);
		}
	}
	return FALSE;
}

BOOL fipImage::rotate(double angle) {
	if(_dib) {
		switch(FreeImage_GetBPP(_dib)) {
			case 1:
			case 8:
			case 24:
			case 32:
				FIBITMAP *rotated = FreeImage_RotateClassic(_dib, angle);
				return replace(rotated);
				break;
		}
	}
	return FALSE;
}

BOOL fipImage::flipVertical() {
	if(_dib) {
		_bHasChanged = TRUE;

		return FreeImage_FlipVertical(_dib);
	}
	return FALSE;
}

BOOL fipImage::flipHorizontal() {
	if(_dib) {
		_bHasChanged = TRUE;

		return FreeImage_FlipHorizontal(_dib);
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////
// Color manipulation routines

BOOL fipImage::invert() {
	if(_dib) {
		_bHasChanged = TRUE;

		return FreeImage_Invert(_dib);
	}
	return FALSE;
}

BOOL fipImage::adjustCurve(BYTE *LUT, FREE_IMAGE_COLOR_CHANNEL channel) {
	if(_dib) {
		_bHasChanged = TRUE;

		return FreeImage_AdjustCurve(_dib, LUT, channel);
	}
	return FALSE;
}

BOOL fipImage::adjustGamma(double gamma) {
	if(_dib) {
		_bHasChanged = TRUE;

		return FreeImage_AdjustGamma(_dib, gamma);
	}
	return FALSE;
}

BOOL fipImage::adjustBrightness(double percentage) {
	if(_dib) {
		_bHasChanged = TRUE;

		return FreeImage_AdjustBrightness(_dib, percentage);
	}
	return FALSE;
}

BOOL fipImage::adjustContrast(double percentage) {
	if(_dib) {
		_bHasChanged = TRUE;

		return FreeImage_AdjustContrast(_dib, percentage);
	}
	return FALSE;
}

BOOL fipImage::getHistogram(DWORD *histo, FREE_IMAGE_COLOR_CHANNEL channel) const {
	if(_dib) {
		return FreeImage_GetHistogram(_dib, histo, channel);
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////
// Upsampling / downsampling routine

BOOL fipImage::rescale(WORD new_width, WORD new_height, FREE_IMAGE_FILTER filter) {
	if(_dib) {
		switch(FreeImage_GetImageType(_dib)) {
			case FIT_BITMAP:
			case FIT_UINT16:
			case FIT_RGB16:
			case FIT_RGBA16:
			case FIT_FLOAT:
			case FIT_RGBF:
			case FIT_RGBAF:
				break;
			default:
				return FALSE;
				break;
		}

		// Perform upsampling / downsampling
		FIBITMAP *dst = FreeImage_Rescale(_dib, new_width, new_height, filter);
		return replace(dst);
	}
	return FALSE;
}

BOOL fipImage::makeThumbnail(WORD max_size, BOOL convert) {
	if(_dib) {
		switch(FreeImage_GetImageType(_dib)) {
			case FIT_BITMAP:
			case FIT_UINT16:
			case FIT_RGB16:
			case FIT_RGBA16:
			case FIT_FLOAT:
			case FIT_RGBF:
			case FIT_RGBAF:
				break;
			default:
				return FALSE;
				break;
		}

		// Perform downsampling
		FIBITMAP *dst = FreeImage_MakeThumbnail(_dib, max_size, convert);
		return replace(dst);
	}
	return FALSE;
}

fipMemoryIO::fipMemoryIO(BYTE *data, DWORD size_in_bytes) {
	_hmem = FreeImage_OpenMemory(data, size_in_bytes);
}

fipMemoryIO::~fipMemoryIO() {
	FreeImage_CloseMemory(_hmem);
}

BOOL fipMemoryIO::isValid() const {
	return (_hmem != NULL);
}

FREE_IMAGE_FORMAT fipMemoryIO::getFileType() const {
	if(_hmem != NULL) {
		return FreeImage_GetFileTypeFromMemory(_hmem, 0);
	}

	return FIF_UNKNOWN;
}

FIBITMAP* fipMemoryIO::load(FREE_IMAGE_FORMAT fif, int flags) const {
	return FreeImage_LoadFromMemory(fif, _hmem, flags);
}

BOOL fipMemoryIO::save(FREE_IMAGE_FORMAT fif, FIBITMAP *dib, int flags) {
	return FreeImage_SaveToMemory(fif, dib, _hmem, flags);
}

unsigned fipMemoryIO::read(void *buffer, unsigned size, unsigned count) const {
	return FreeImage_ReadMemory(buffer, size, count, _hmem);
}

unsigned fipMemoryIO::write(const void *buffer, unsigned size, unsigned count) {
	return FreeImage_WriteMemory(buffer, size, count, _hmem);
}

long fipMemoryIO::tell() const {
	return FreeImage_TellMemory(_hmem);
}

BOOL fipMemoryIO::seek(long offset, int origin) {
	return FreeImage_SeekMemory(_hmem, offset, origin);
}

BOOL fipMemoryIO::acquire(BYTE **data, DWORD *size_in_bytes) {
	return FreeImage_AcquireMemory(_hmem, data, size_in_bytes);
}

//////////////////////////////////////////////////////////////////////

struct Image::impl
{
	fipImage m_image;
	ImageType m_type;

	impl()
	{
		m_type = itUnknown;
	}

	ImageType getType() const
	{
		return m_type;
	}

	uint32 getWidth() const
	{
		return static_cast<uint32>(m_image.getWidth());
	}

	uint32 getHeight() const
	{
		return static_cast<uint32>(m_image.getHeight());
	}

	bool load(const Buffer &buffer, ImageType type)
	{
		fipMemoryIO mem(buffer.getData(), buffer.getSize());
		m_type = convert(mem.getFileType());

		return m_image.loadFromMemory(mem) == TRUE;
	}

	bool save(Buffer &buffer, ImageType type)
	{
		buffer.clear();

		fipMemoryIO mem;
		if(m_image.saveToMemory(convert(type != itUnknown ? type : m_type), mem))
		{
			BYTE *data = null;
			DWORD size = 0;
			if(mem.acquire(&data, &size))
                return buffer.write(data, size) == size;
		}

		return false;
	}

	static FREE_IMAGE_FORMAT convert(ImageType type)
	{
		switch(type)
		{
		case itUnknown:		return FIF_UNKNOWN;
		case itBmp:			return FIF_BMP;
		case itIco:			return FIF_ICO;
		case itJpg:			return FIF_JPEG;
		case itGif:			return FIF_GIF;
		case itPcx:			return FIF_PCX;
		case itPng:			return FIF_PNG;
		case itTiff:		return FIF_TIFF;
		}

		return FIF_UNKNOWN;
	}

	static ImageType convert(FREE_IMAGE_FORMAT type)
	{
		switch(type)
		{
		case FIF_UNKNOWN:	return itUnknown;
		case FIF_BMP:		return itBmp;
		case FIF_ICO:		return itIco;
		case FIF_JPEG:		return itJpg;
		case FIF_GIF:		return itGif;
		case FIF_PCX:		return itPcx;
		case FIF_PNG:		return itPng;
		case FIF_TIFF:		return itTiff;
		}

		return itUnknown;
	}
};

//////////////////////////////////////////////////////////////////////

Image::Image()
{

}

Image::Image(const String &filename, ImageType type)
{
	load(filename, type);
}

Image::~Image()
{

}

Image::ImageType Image::getType() const
{
	return m_impl->getType();
}

uint32 Image::getWidth() const
{
	return m_impl->getWidth();
}

uint32 Image::getHeight() const
{
	return m_impl->getHeight();
}

bool Image::load(const String &filename, ImageType type)
{
	Buffer buffer;
	if(buffer.load(filename) == false)
		return false;

	return load(buffer, type);
}

bool Image::load(const Buffer &buffer, ImageType type)
{
	return m_impl->load(buffer, type);
}

bool Image::save(const String &filename, ImageType type)
{
	Buffer buffer;
	if(save(buffer, type) == false)
		return false;

	return buffer.save(filename);
}

bool Image::save(Buffer &buffer, ImageType type)
{
	return m_impl->save(buffer, type);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
