/*
 * Copyright 2011-2014 Branimir Karadzic. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-2-Clause
 */

#ifndef BGFX_GLCONTEXT_EAGL_H_HEADER_GUARD
#define BGFX_GLCONTEXT_EAGL_H_HEADER_GUARD

#if BX_PLATFORM_IOS

namespace bgfx
{
	struct SwapChainGL;

	struct GlContext
	{
		GlContext()
			: m_context(0)
		{
		}

		void create(uint32_t _width, uint32_t _height);
		void destroy();
		void resize(uint32_t _width, uint32_t _height, bool _vsync);

		static bool isSwapChainSupported();
		SwapChainGL* createSwapChain(void* _nwh);
		void destorySwapChain(SwapChainGL*  _swapChain);
		void swap(SwapChainGL* _swapChain = NULL);
		void makeCurrent(SwapChainGL* _swapChain = NULL);

		void import();

		GLuint getFbo()
		{
			return m_fbo;
		}

		bool isValid() const
		{
			return 0 != m_context;
		}

		void* m_view;
		void* m_context;

		GLuint m_fbo;
		GLuint m_colorRbo;
		GLuint m_depthStencilRbo;
	};
} // namespace bgfx

#endif // BX_PLATFORM_IOS

#endif // BGFX_GLCONTEXT_EAGL_H_HEADER_GUARD
