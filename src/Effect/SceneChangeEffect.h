/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_SCENECHANGEEFFECT_H
#define __GLAZIERY_SCENECHANGEEFFECT_H


namespace glaziery
{
	/**
	 * TODO
	 */
	class SceneChangeEffect : public Effect
	{
		private:

			/**
			 * Whether to change the scene, even if the effect is canceled.
			 */
			bool changeOnCancel;

			/**
			 * The ID of the scene to change to.
			 */
			const char * sceneId;

		protected:

			/**
			 * Performs operations for one frame, advancing the effect by the frame time.
			 * Afterwards, the effect issues whether it is complete now.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 * @param time This frame's time in milli seconds. You should advance the
			 * effect by this amount.
			 * @return Whether to continue the effect. false will destroy it.
			 * @note This method is called by the Desktop. Do not call it directly.
			 */
			virtual bool execute(DrawingContext & context, long time);

			/**
			 * Performs operations if the effect now ends because it was canceled.
			 * @param context The drawing context that specifies the surface and clipping
			 * rectangle for the drawing operation.
			 */
			virtual void onCancel(DrawingContext & context);

		// Runtime class macros require their own public section.
		public:

			#if defined(_DEBUG) && (defined(_AFX) || defined(_AFXDLL))
				DECLARE_DYNAMIC(SceneChangeEffect);
			#endif

		public:

			/**
			 * Creates a new scene change effect. After creation, add the effect to the Desktop.
			 * @param scene The scene to change to.
			 * @param changeOnCancel Whether to change the scene, even if the effect is canceled.
			 * @see Desktop::addEffect()
			 */
			SceneChangeEffect(Scene * scene, bool changeOnCancel = false);

			/**
			 * Creates a new scene change effect. After creation, add the effect to the Desktop.
			 * @param sceneId The ID of the scene to change to.
			 * @param changeOnCancel Whether to change the scene, even if the effect is canceled.
			 * @see Desktop::addEffect()
			 */
			SceneChangeEffect(const char * sceneId, bool changeOnCancel = false);
	};
}


#endif
