#ifndef __FRAMEBUFFER__
#define __FRAMEBUFFER__

#include <gl\gl.h>

class FrameBuffer
{
public:
	FrameBuffer  ( int width, int height, int flags = 0 );
	~FrameBuffer ();

	int getWidth  () const;
	int getHeight () const;

	bool hasStencil () const;
	bool hasDepth   () const;

	bool isOk () const;
	bool create ();
	bool bind   ();
	bool unbind ();

	bool attachColorTexture ( GLenum target, unsigned textureId, int no = 0 );
	bool attachDepthTexture ( GLenum target, unsigned textureId );

	bool detachColorTexture ( GLenum target );
	bool detachDepthTexture ( GLenum target );

	static int maxColorAttachements ();

	enum {
		DEPTH16 = 1,
		DEPTH24 = 2,
		DEPTH32 = 4,

		STENCIL1 = 16,
		STENCIL4 = 32,
		STENCIL8 = 64,
		STENCIL16 = 128
	};

private:
	int      flags_;
	int      width_;
	int      height_;
	unsigned frameBuffer_;
	unsigned depthBuffer_;
	unsigned stencilBuffer_;
};

#endif