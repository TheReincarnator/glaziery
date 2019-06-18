/*
 * This file is part of the Glaziery.
 * Copyright Thomas Jacob.
 *
 * READ README.TXT BEFORE USE!!
 */


#ifndef __GLAZIERY_DIRECTXPLATFORMADAPTER_INL
#define __GLAZIERY_DIRECTXPLATFORMADAPTER_INL


inline void DirectXPlatformAdapter::addTextureRectangle(Vector sourcePosition, Vector sourceSize,
	Vector targetPosition, Vector targetSize, unsigned long modulationColor)
{
	addTextureRectangle(sourcePosition.x, sourcePosition.y, sourceSize.x, sourceSize.y,
		(float) targetPosition.x, (float) targetPosition.y,
		(float) (targetPosition.x + targetSize.x), (float) targetPosition.y,
		(float) targetPosition.x, (float) (targetPosition.y + targetSize.y),
		(float) (targetPosition.x + targetSize.x), (float) (targetPosition.y + targetSize.y),
		modulationColor);
}

inline void DirectXPlatformAdapter::addTextureRectangle(int sourceX, int sourceY, int sourceWidth, int sourceHeight,
	float targetX, float targetY, float targetWidth, float targetHeight, unsigned long modulationColor)
{
	addTextureRectangle(sourceX, sourceY, sourceWidth, sourceHeight,
		targetX, targetY, targetX + targetWidth, targetY, targetX, targetY + targetHeight,
		targetX + targetWidth, targetY + targetHeight, modulationColor);
}

inline void DirectXPlatformAdapter::addTextureRectangle(Vector sourcePosition, Vector sourceSize,
	Vector targetPosition0, Vector targetPosition1, Vector targetPosition2,
	Vector targetPosition3, unsigned long modulationColor)
{
	addTextureRectangle(sourcePosition.x, sourcePosition.y, sourceSize.x, sourceSize.y,
		(float) targetPosition0.x, (float) targetPosition0.y, (float) targetPosition1.x, (float) targetPosition1.y,
		(float) targetPosition2.x, (float) targetPosition2.y, (float) targetPosition3.x, (float) targetPosition3.y,
		modulationColor);
}

inline void DirectXPlatformAdapter::addTextureRectangle(int sourceX, int sourceY, int sourceWidth, int sourceHeight,
	float target0X, float target0Y, float target1X, float target1Y,
	float target2X, float target2Y, float target3X, float target3Y, unsigned long modulationColor)
{
	if (vertexNo + 6 > VERTEX_BUFFER_SIZE)
		if (!flushVertexBuffer())
			return;

	if (vertexNo == 0)
	{
		if (failedObserving(vertexBuffer->Lock(0, VERTEX_BUFFER_SIZE * sizeof(Point), (void **) &currentVertices,
			D3DLOCK_DISCARD), "Cannot lock vertex buffer"))
			return;

		::ZeroMemory(currentVertices, VERTEX_BUFFER_SIZE * sizeof(Point));
	}

	int vertex0 = vertexNo++;
	currentVertices[vertex0].x = target0X - 0.5f;
	currentVertices[vertex0].y = target0Y - 0.5f;
	currentVertices[vertex0].z = 0.5f;
	currentVertices[vertex0].rhw = 1.0f;
	currentVertices[vertex0].diffuse = modulationColor;
	currentVertices[vertex0].tu = sourceX * 1.0f / currentTextureSize.x;
	currentVertices[vertex0].tv = sourceY * 1.0f / currentTextureSize.y;

	int vertex1 = vertexNo++;
	currentVertices[vertex1].x = target1X - 0.5f;
	currentVertices[vertex1].y = target1Y - 0.5f;
	currentVertices[vertex1].z = 0.5f;
	currentVertices[vertex1].rhw = 1.0f;
	currentVertices[vertex1].diffuse = modulationColor;
	currentVertices[vertex1].tu = (sourceX + sourceWidth) * 1.0f / currentTextureSize.x;
	currentVertices[vertex1].tv = currentVertices[vertex0].tv;

	int vertex2 = vertexNo++;
	currentVertices[vertex2].x = target2X - 0.5f;
	currentVertices[vertex2].y = target2Y - 0.5f;
	currentVertices[vertex2].z = 0.5f;
	currentVertices[vertex2].rhw = 1.0f;
	currentVertices[vertex2].diffuse = modulationColor;
	currentVertices[vertex2].tu = currentVertices[vertex0].tu;
	currentVertices[vertex2].tv = (sourceY + sourceHeight) * 1.0f / currentTextureSize.y;

	int vertex3 = vertexNo++;
	int vertex4 = vertexNo++;
	::CopyMemory(&currentVertices[vertex3], &currentVertices[vertex1], 2 * sizeof(Point));

	int vertex5 = vertexNo++;
	currentVertices[vertex5].x = target3X - 0.5f;
	currentVertices[vertex5].y = target3Y - 0.5f;
	currentVertices[vertex5].z = 0.5f;
	currentVertices[vertex5].rhw = 1.0f;
	currentVertices[vertex5].diffuse = modulationColor;
	currentVertices[vertex5].tu = currentVertices[vertex1].tu;
	currentVertices[vertex5].tv = currentVertices[vertex2].tv;
}

inline bool DirectXPlatformAdapter::failedObserving(HRESULT result, const char * message)
{
	if (FAILED(result))
	{
		handleFailure(result, message);
		return true;
	}

	return false;
}

inline bool DirectXPlatformAdapter::flushVertexBuffer()
{
	if (vertexNo <= 0)
		{return true;}

	if (failedObserving(vertexBuffer->Unlock(), "Cannot unlock vertex buffer"))
		{return false;}

	if (failedObserving(direct3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, vertexNo / 3), "Cannot draw primitives"))
		{return false;}

	vertexNo = 0;
	return true;
}

inline bool DirectXPlatformAdapter::setTexture(Image * image, BlendType blendType)
{
	if (direct3DDevice == NULL)
	{
		currentTexture = NULL;
		return true;
	}

	if (image == NULL)
	{
		if (currentTexture != NULL || currentBlendType != blendType)
			flushVertexBuffer();

		if (currentTexture != NULL)
		{
			if (failedObserving(direct3DDevice->SetTexture(0, NULL), "Cannot unset texture"))
				return false;

			currentTexture = NULL;
		}
	}
	else
	{
		DirectXImage * directXImage = (DirectXImage *) image;
		IDirect3DTexture9 * texture = directXImage->getTexture();
		if (currentTexture != texture || currentBlendType != blendType)
			flushVertexBuffer();

		if (currentTexture != texture)
		{
			if (failedObserving(direct3DDevice->SetTexture(0, texture), "Cannot set texture"))
				return false;
			currentTexture = texture;
			currentTextureSize = directXImage->getTextureSize();
		}
	}

	if (currentBlendType != blendType)
	{
		if (blendType == BLENDTYPE_NORMAL)
		{
			// Normal color blending is Sc * Sa + Dc * (1-Sa).
			// This ensures that texture is transparent or opaque according to their alpha channel.
			if (failedObserving(direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA),
				"Cannot set source color blending"))
				return false;
			if (failedObserving(direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA),
				"Cannot set destination color blending"))
				return false;

			if (alphaFullySupported)
			{
				// Normal alpha blending is Sa * 1 + Da * (1-Sa) = Sa + Da - Sa * Da.
				// This is smooth-adding of alpha values to give the resulting texture a natural transparency,
				// if it is used for drawing on other surfaces again. E.g., 0.5 and 0.5 result in 1.
				if (failedObserving(direct3DDevice->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA),
					"Cannot set destination alpha blending"))
					return false;
			}

			// Use texture and modulation color (arg1 is the texture, arg2 is the diffuse modulation color).
			if (failedObserving(direct3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE),
				"Cannot set texture stage color operation"))
				return false;
		}
		else if (blendType == BLENDTYPE_OPAQUE)
		{
			// Opaque color blending is Sc * 1 + Dc * 0.
			if (failedObserving(direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE),
				"Cannot set source color blending"))
				return false;
			if (failedObserving(direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO),
				"Cannot set destination color blending"))
				return false;

			if (alphaFullySupported)
			{
				// Do not use the destination alpha information.
				if (failedObserving(direct3DDevice->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO),
					"Cannot set destination alpha blending"))
					return false;
			}

			// Use texture and modulation color (arg1 is the texture, arg2 is the diffuse modulation color).
			if (failedObserving(direct3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE),
				"Cannot set texture stage color operation"))
				return false;
		}
		else if (blendType == BLENDTYPE_STENCIL)
		{
			// Do not use the texture color, leave the target color as it is.
			if (failedObserving(direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO),
				"Cannot set source color blending"))
				return false;
			if (failedObserving(direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE),
				"Cannot set destination color blending"))
				return false;

			if (alphaFullySupported)
			{
				// Render the source alpha opaquely, without using the target alpha.
				if (failedObserving(direct3DDevice->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO),
					"Cannot set destination alpha blending"))
					return false;
			}

			// Use texture and modulation color (arg1 is the texture, arg2 is the diffuse modulation color).
			if (failedObserving(direct3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE),
				"Cannot set texture stage color operation"))
				return false;
		}
		else if (blendType == BLENDTYPE_SILHOUETTE)
		{
			// Silhouette color blending (like normal color blending) is Sc * Sa + Dc * (1-Sa).
			// This ensures that texture are transparent or opaque according to their alpha channel.
			if (failedObserving(direct3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA),
				"Cannot set source color blending"))
				return false;
			if (failedObserving(direct3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA),
				"Cannot set destination color blending"))
				return false;

			if (alphaFullySupported)
			{
				// Stencil alpha blending (like normal alpha blending) is Sa * 1 + Da * (1-Sa) = Sa + Da - Sa * Da.
				// This is smooth-adding of alpha values to give the resulting texture a natural transparency,
				// if it is used for drawing on other surfaces again. E.g., 0.5 and 0.5 result in 1.
				if (failedObserving(direct3DDevice->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA),
					"Cannot set destination alpha blending"))
					return false;
			}

			// Do not use the texture color (arg1 is the texture, arg2 is the diffuse modulation color).
			if (failedObserving(direct3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG2),
				"Cannot set texture stage color operation"))
				return false;
		}

		currentBlendType = blendType;
	}

	return true;
}


#endif
