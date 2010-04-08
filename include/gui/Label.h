/******************************************************************************
 ** Copyright (c) 2010 Seed Developers
 ** All rights reserved
 ** Contact: licensing@seedframework.org
 ** Website: http://www.seedframework.org
 
 ** This file is part of the Seed Framework.
 
 ** Commercial Usage
 ** Seed Framework is available under proprietary license for those who cannot,
 ** or choose not to, use LGPL and GPL code in their projects (eg. iPhone,
 ** Nintendo Wii and others).
 
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Seed developers gives you certain
 ** additional rights. These rights are described in the Seed Framework LGPL
 ** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
 ** package.
 **
 ** If you have questions regarding the use of this file, please contact
 ** Seed developers at licensing@seedframework.org.
 **
 *****************************************************************************/

/*! \file Label.h
	\author Danny Angelo Carminati Grein
	\brief GUI Label Widget
*/

#ifndef __GUI_LABEL_H__
#define __GUI_LABEL_H__

#include "Text.h"
#include "GuiManager.h"
#include "../interface/IWidget.h"


namespace Seed {


class Label : public IWidget
{
	public:
		Label();
		virtual ~Label();

		virtual void Reset();
		virtual void Update(f32 dt);

		virtual void SetAutoAdjust(BOOL b);
		virtual BOOL IsAutoAdjust() const;
		virtual void SetAlignment(eHorizontalAlignment align);
		virtual void SetText(u32 dictId);
		virtual void SetText(WideString str);
		virtual void SetText(const String &str);
		//virtual void Print(WideString str, ...);
		virtual void SetFont(const Font *font);

		f32 GetTextWidth();

		// IRenderable
		virtual void Render();
		virtual void SetColor(u8 r, u8 g, u8 b, u8 a);
		virtual void SetColor(PIXEL px);

		// ITransformable2D
		virtual void SetWidth(f32 w);

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

	protected:
		eHorizontalAlignment	iAlign;
		PIXEL		iColor;
		BOOL		bAutoAdjust;
		Text		cText;
		const Font	*pFont;

	private:
		SEED_DISABLE_COPY(Label);
};


} // namespace


#endif // __LABEL_H__

