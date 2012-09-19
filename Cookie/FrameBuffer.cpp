#include <ExtensionGL.h>

#include "FrameBuffer.h"

FrameBuffer::FrameBuffer ( int width, int height, int flags )
{
	width_         = width;
	height_        = height;
	flags_         = flags;
	frameBuffer_   = 0;
	depthBuffer_   = 0;
	stencilBuffer_ = 0;
}

FrameBuffer::~FrameBuffer ()
{
	if ( depthBuffer_ != 0 )
		glDeleteRenderbuffers ( 1, &depthBuffer_ );

	if ( stencilBuffer_ != 0 )
		glDeleteRenderbuffers ( 1, &stencilBuffer_ );

	if ( frameBuffer_ != 0 )
		glDeleteFramebuffers ( 1, &frameBuffer_ );
}

int FrameBuffer::getWidth () const
{
	return width_;
}

int FrameBuffer::getHeight () const
{
	return height_;
}

bool FrameBuffer::hasStencil () const
{
	return stencilBuffer_ != 0;
}

bool FrameBuffer::hasDepth () const
{
	return depthBuffer_ != 0;
}

bool FrameBuffer::isOk () const
{
	unsigned currentFb;

	glGetIntegerv ( GL_FRAMEBUFFER_BINDING, (int *)&currentFb );

	if ( currentFb != frameBuffer_ )
		glBindFramebuffer ( GL_FRAMEBUFFER, frameBuffer_ );

	bool complete = glCheckFramebufferStatus ( GL_FRAMEBUFFER ) ==
		                                       GL_FRAMEBUFFER_COMPLETE;

	if ( currentFb != frameBuffer_ )
		glBindFramebuffer ( GL_FRAMEBUFFER, currentFb );

	return complete;
}

bool FrameBuffer::create ()
{
	if ( width_ <= 0 || height_ <= 0 || width_ >= GL_MAX_RENDERBUFFER_SIZE ||
		 height_ >= GL_MAX_RENDERBUFFER_SIZE )
		 return false;

	glGenFramebuffers ( 1, &frameBuffer_ );
	glBindFramebuffer ( GL_FRAMEBUFFER, frameBuffer_ );

	int depthFormat   = 0;
	int stencilFormat = 0;

	if ( flags_ & DEPTH16 )
		depthFormat = GL_DEPTH_COMPONENT16;
	else if ( flags_ & DEPTH24 )
		depthFormat = GL_DEPTH_COMPONENT24;
	else if ( flags_ & DEPTH32 )
		depthFormat = GL_DEPTH_COMPONENT32;

	if ( flags_ & STENCIL1 )
		stencilFormat = GL_STENCIL_INDEX1;
	else if ( flags_ & STENCIL4 )
		stencilFormat = GL_STENCIL_INDEX4;
	else if ( flags_ & STENCIL8 )
		stencilFormat = GL_STENCIL_INDEX8;
	else if ( flags_ & STENCIL16 )
		stencilFormat = GL_STENCIL_INDEX16;

	if ( depthFormat != 0 ) {
		glGenRenderbuffers        ( 1, &depthBuffer_ );
		glBindRenderbuffer        ( GL_RENDERBUFFER, depthBuffer_ );
		glRenderbufferStorage     ( GL_RENDERBUFFER, depthFormat, width_,
			                                                      height_ );
		glFramebufferRenderbuffer ( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			                        GL_RENDERBUFFER, depthBuffer_ );
	}

	if ( stencilFormat != 0 ) {
		glGenRenderbuffers        ( 1, &stencilBuffer_ );
		glBindRenderbuffer        ( GL_RENDERBUFFER, stencilBuffer_ );
		glRenderbufferStorage     ( GL_RENDERBUFFER, stencilFormat, width_,
			                                                        height_ );
		glFramebufferRenderbuffer ( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
			                        GL_RENDERBUFFER, stencilBuffer_ );
	}

	bool complete = glCheckFramebufferStatus ( GL_FRAMEBUFFER ) ==
		                                       GL_FRAMEBUFFER_COMPLETE;

	glBindFramebuffer ( GL_FRAMEBUFFER, 0 );

	return complete;
}

bool FrameBuffer::bind ()
{
	if ( frameBuffer_ == 0 )
		return false;

	glBindFramebuffer ( GL_FRAMEBUFFER, frameBuffer_ );

	return true;
}

bool FrameBuffer::unbind ()
{
	if ( frameBuffer_ == 0 )
		return false;

	glBindFramebuffer ( GL_FRAMEBUFFER, 0 );
	
	return true;
}

bool FrameBuffer::attachColorTexture ( GLenum target, unsigned textureId, int no )
{
	if ( frameBuffer_ == 0 )
		return false;

	if ( target != GL_TEXTURE_2D && (target < GL_TEXTURE_CUBE_MAP_POSITIVE_X ||
		                             target > GL_TEXTURE_CUBE_MAP_NEGATIVE_Z) )
		return false;

	glFramebufferTexture2D ( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + no,
		                     target, textureId, 0 );

	return true;
}

bool FrameBuffer::attachDepthTexture ( GLenum target, unsigned textureId )
{
	if ( frameBuffer_ == 0 )
		return false;

	glFramebufferTexture2D ( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		                     target, textureId, 0 );

	return true;
}

bool FrameBuffer::detachColorTexture ( GLenum target )
{
	return attachColorTexture ( target, 0 );
}

bool FrameBuffer::detachDepthTexture ( GLenum target )
{
	return attachDepthTexture ( target, 0 );
}

int FrameBuffer::maxColorAttachements ()
{
	int n;

	glGetIntegerv ( GL_MAX_COLOR_ATTACHMENTS, &n );

	return n;
}