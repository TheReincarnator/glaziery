/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_CACHABLECOMPONENT_H
#define __GLAZIERY_CACHABLECOMPONENT_H


namespace glaziery
{
	/**
	 * A cachable component is a component that only renders its content
	 * if it has changed. To get notified of changes, the component either
	 * receives invalidate() events when the model changes, or the implementation
	 * of drawContent() calls invalidate() every time to declare that the content
	 * must be drawn every frame, because the state does not depend on change events
	 * or the events cannot be pushed into the application or game.
	 * @see Component::invalidate()
	 */
	class CachableComponent : public Component
	{
		private:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				class RedrawRegion : public CObject
			#else
				class RedrawRegion
			#endif
			{
				public:

					Vector position;
					Vector size;
					unsigned long time;

					RedrawRegion(const Vector & position, const Vector & size);
			};

			/**
			 * A listener for the cache surface.
			 */
			class SurfaceListener : public Image::Listener
			{
				private:

					/**
					 * The cachable component this listener listens for.
					 */
					CachableComponent * component;

				public:

					/**
					 * Handles the event that a render target image has been invalidated,
					 * e.g. the physical device has been lost.
					 * @param renderTarget The invalidated render target.
					 */
					virtual void onRenderTargetInvalidated(Image * renderTarget);

					/**
					 * Creates a new surface listener.
					 * @param component The cachable component this listener listens for.
					 */
					SurfaceListener(CachableComponent * component);
			};

		friend SurfaceListener;

		private:

			/**
			 * Whether to cache the content of this component. Defaults to true.
			 * @warning Disabling the caching also results in graphical errors, because
			 * there is no image to create alpha blending effects on.
			 */
			bool caching;

			/**
			 * The alpha blending value, defining the grade of transparency
			 * of this component, if it has focus (is active).
			 * An alpha value of 0 represents complete transparency
			 * (i.e. the component is invisible), and 255 represents complete opaqueness.
			 */
			unsigned char focusAlpha;

			/**
			 * The alpha blending value, defining the grade of transparency
			 * of this component, if it has no focus (is inactive).
			 * An alpha value of 0 represents complete transparency
			 * (i.e. the component is invisible), and 255 represents complete opaqueness.
			 */
			unsigned char nonFocusAlpha;

			#ifdef GL_DEBUG_INVALIDATIONS
				ArrayList<RedrawRegion> redrawRegions;
			#endif

			/**
			 * The image rendering cache surface.
			 * This surface is used to render the component to.
			 * When the component is drawn, the valid attribute is checked.
			 * If the component is invalid, the surface is cleared and redrawn.
			 * After that, the surface represents a component cache and it used
			 * to blit the whole component every frame without redrawing it.
			 * @see valid
			 * @see draw()
			 */
			Image * surface;

			/**
			 * Whether the component surface represents the current state
			 * of the component, including the client area.
			 * Any change to the component must be followed by a call to invalidate(),
			 * setting the valid property to false. The next window surface
			 * rendering then sets this property to true again.
			 * If valid is false, the invalidAreaPosition and invalidAreaSize attributes
			 * contain the area to be rerendered.
			 */
			bool valid;

			/**
			 * Whether this component caches on an image with alpha channel.
			 * Defaults to the ability of the system to fully support alpha.
			 * @see PlatformAdapter::isAlphaFullySupported()
			 */
			bool withAlphaChannel;

			/**
			 * Handles the event that a render target image owned by this event target
			 * has been invalidates, e.g. the physical device has been lost.
			 * @param renderTarget The invalidated render target.
			 */
			void onRenderTargetInvalidated(Image * renderTarget);

		protected:

			/**
			 * The top-left position, relative to this component's position, of the invalid area,
			 * if any area is invalid (the valid flag is set).
			 */
			Vector invalidAreaPosition;

			/**
			 * The top-left position, relative to this component's position, of the invalid area,
			 * if any area is invalid (the valid flag is set).
			 */
			Vector invalidAreaSize;

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(CachableComponent);
			#endif

		public:

			/**
			 * Creates a new cachable component.
			 */
			CachableComponent();

			/**
			 * Destroys the cachable component.
			 */
			virtual ~CachableComponent();

			/**
			 * Draws the component to the display surface.
			 * This is usually achieved by delegating to the current skin.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @note The caller is responsible for not calling this method
			 * if the component is invisible.
			 * @see Desktop::getSkin()
			 */
			virtual void draw(DrawingContext & context);

			/**
			 * Draws the content of the component to the display surface.
			 * This is usually achieved by delegating to the current skin.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @note The implementation must clear the context before rendering,
			 * if it does not render to the entire context.
			 * @note This method is called when the cache is invalid only.
			 * See invalidate() and the CachableComponent's class comment for details.
			 * If the cache never gets valid (because the display depends on some event
			 * that is not pushed into the application or game), call invalidate() from
			 * within the implementation of drawContent().
			 * @see Desktop::getSkin()
			 * @see CachableComponent::invalidate()
			 * @see CachableComponent
			 */
			virtual void drawContent(DrawingContext & context) = NULL;

			/**
			 * Returns the alpha blending value, defining the grade of transparency
			 * of this component. Checks whether the component has focus and uses
			 * the appropriate alpha value.
			 * An alpha value of 0 represents complete transparency
			 * (i.e. the component is invisible), and 255 represents complete opaqueness.
			 * @return The alpha value.
			 */
			unsigned char getAlpha();

			/**
			 * Returns the alpha blending value, defining the grade of transparency
			 * of this component, if it has focus (is active).
			 * An alpha value of 0 represents complete transparency
			 * (i.e. the component is invisible), and 255 represents complete opaqueness.
			 * @return The alpha value.
			 */
			unsigned char getFocusAlpha();

			/**
			 * Returns the modulation color used when actually rendering the cachable component.
			 * Defaults to white with the alpha value returned by getAlpha().
			 * @return The modulation color for rendering.
			 */
			virtual unsigned long getModulationColor();

			/**
			 * Returns the alpha blending value, defining the grade of transparency
			 * of this component, if it has no focus (is inactive).
			 * An alpha value of 0 represents complete transparency
			 * (i.e. the component is invisible), and 255 represents complete opaqueness.
			 * @return The alpha value.
			 */
			unsigned char getNonFocusAlpha();

			/**
			 * Hides the component (makes it invisible, but still existent).
			 */
			virtual void hide();

			/**
			 * Invalidates a part of this component's internal caches (e.g. drawing caches).
			 * This method should be called whenever its state changes.
			 * The invalidation is propagated to the parent, if any.
			 * @param position The top-left position of the area to be invalidated,
			 * relative to this component.
			 * @param size The size of the area to be invalidated.
			 */
			virtual void invalidateArea(Vector position, Vector size);

			/**
			 * Returns whether to cache the content of this component. Defaults to true.
			 * @warning Disabling the caching also results in graphical errors, because
			 * there is no image to create alpha blending effects on.
			 * @return Whether to cache the content of this component.
			 */
			bool isCaching();

			/**
			 * Returns whether the component surface represents the current state
			 * of the component, including the client area.
			 * Any change to the component must be followed by a call to invalidate(),
			 * setting the valid property to false. The next component surface
			 * rendering then sets this property to true again.
			 * @return Whether the component surface is up to date.
			 * @see invalidate()
			 */
			bool isValid();

			/**
			 * Returns whether this component caches on an image with alpha channel.
			 * Defaults to the ability of the system to fully support alpha.
			 * @return Whether this component caches on an image with alpha channel.
			 * @see PlatformAdapter::isAlphaFullySupported()
			 */
			bool isWithAlphaChannel();

			/**
			 * Invalidates and releases this component's internal caches (e.g. drawing caches).
			 * This method should be called when it is unlikely that the cache will be needed
			 * in the near future, to release system resources (e.g. when the component gets invisible).
			 */
			virtual void releaseCache();

			/**
			 * Sets whether to cache the content of this component. Defaults to true.
			 * @warning Disabling the caching also results in graphical errors, because
			 * there is no image to create alpha blending effects on.
			 * @param caching Whether to cache the content of this component.
			 */
			void setCaching(bool caching);

			/**
			 * Sets the alpha blending value, defining the grade of transparency
			 * of this component, if it has focus (is active).
			 * An alpha value of 0 represents complete transparency
			 * (i.e. the component is invisible), and 255 represents complete opaqueness.
			 * @param focusAlpha The alpha value.
			 */
			void setFocusAlpha(unsigned char focusAlpha);

			/**
			 * Sets the alpha blending value, defining the grade of transparency
			 * of this component, if it has no focus (is inactive).
			 * An alpha value of 0 represents complete transparency
			 * (i.e. the component is invisible), and 255 represents complete opaqueness.
			 * @param nonFocusAlpha The alpha value.
			 */
			void setNonFocusAlpha(unsigned char nonFocusAlpha);

			/**
			 * Sets whether this component caches on an image with alpha channel.
			 * Defaults to the ability of the system to fully support alpha.
			 * @param withAlphaChannel Whether this component caches on an image with alpha channel.
			 * @see PlatformAdapter::isAlphaFullySupported()
			 */
			void setWithAlphaChannel(bool withAlphaChannel);

			/**
			 * Returns a string reprensentation of this object.
			 * @return The string representation.
			 */
			virtual String toString();
	};
}


#endif
