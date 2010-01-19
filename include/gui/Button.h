/*! \file Button.h
	\author Rafael Eduardo Gonchor
			Danny Angelo Carminati Grein
	\brief GUI Button Widget
*/

#ifndef __GUI_BUTTON_H__
#define __GUI_BUTTON_H__

#include "Sprite.h"
#include "Label.h"
#include "GuiManager.h"
#include "ResourceManager.h"
#include "MemoryManager.h"


namespace Seed {


class ResourceManager;
class SpriteObject;
class CollisionMask;


IResource *ButtonResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);


enum eCollisionType
{
	CollisionNone			= -1, // Default, but is the same than CollisionByRectangle when not set by the user
	CollisionByRectangle	= 0,
	CollisionByMask			= 1,
	CollisionByPixel		= 2
};


class Button : public IWidget, public IResource
{
	friend IResource *ButtonResourceLoader(const char *filename, ResourceManager *res, IMemoryPool *pool);

	public:
		Button();
		virtual ~Button();

		virtual void Initialize(u32 id, eCollisionType type = CollisionByRectangle);
		virtual void Initialize(u32 id, f32 posX, f32 posY, f32 width, f32 height);

		virtual void Select();
		virtual void Unselect();
		virtual void Update(f32 dt);
		virtual void SetDraggable(BOOL b);
		virtual void SetDraggingPriority(u32 i);
		virtual u32 GetDraggingPriority() const;
		virtual void SetDragCentered(BOOL b);
		virtual void SetSprite(const char *spriteName, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual void SetVisible(BOOL b);
		virtual void SetFrameControl(BOOL b);
		virtual void SetEnableHover(BOOL b);
		virtual BOOL IsHoverDisabled() const;
		virtual void SetPriority(u32 p);
		virtual void SetPressOffset(f32 x, f32 y);
		virtual void SetHoverOffset(f32 x, f32 y);

		virtual void SetMask(const char *maskName, IMemoryPool *pool = pDefaultPool);
		virtual void SetCollisionType(eCollisionType type);
		virtual eCollisionType GetCollisionType() const;

		virtual void SetBlending(Seed::IRenderable::eBlendMode op);
		virtual void SetColor(u8 r, u8 g, u8 b, u8 a);
		virtual void SetColor(PIXEL color);

		// IEventWidgetListener
		virtual void OnWidgetRollOver(const EventWidget *ev);
		virtual void OnWidgetRollOut(const EventWidget *ev);
		virtual void OnWidgetPress(const EventWidget *ev);
		virtual void OnWidgetRelease(const EventWidget *ev);
		virtual void OnWidgetReleaseOut(const EventWidget *ev);
		virtual void OnWidgetDrag(const EventWidget *ev);
		virtual void OnWidgetDrop(const EventWidget *ev);

		// ITransformable2D - Label
		virtual void SetLabelWidth(f32 w);
		virtual void SetLabelHeight(f32 h);

		virtual void SetLabelX(f32 x);
		virtual void SetLabelY(f32 y);
		virtual void AddLabelX(f32 value);
		virtual void AddLabelY(f32 value);

		virtual void SetLabelPosition(f32 x, f32 y);
		virtual void AddLabelPosition(f32 x, f32 y);

		virtual void SetLabelRotation(f32 rot);
		virtual void AddLabelRotation(f32 rot);

		virtual void SetLabelScaleX(f32 scaleX);
		virtual void SetLabelScaleY(f32 scaleY);
		virtual void SetLabelScale(f32 scale);
		virtual void SetLabelScale(f32 scaleX, f32 scaleY);

		virtual void AddLabelScaleX(f32 scaleX);
		virtual void AddLabelScaleY(f32 scaleY);
		virtual void AddLabelScale(f32 scale);
		virtual void AddLabelScale(f32 scaleX, f32 scaleY);

		virtual f32 GetLabelWidth() const;
		virtual f32 GetLabelHeight() const;

		virtual f32 GetLabelX() const;
		virtual f32 GetLabelY() const;

		virtual f32 GetLabelRotation() const;

		virtual f32 GetLabelScaleX() const;
		virtual f32 GetLabelScaleY() const;

		// Label
		virtual void SetLabelHorizontalAlignment(eHorizontalAlignment align);
		virtual void SetLabelVerticalAlignment(eVerticalAlignment align);
		virtual void SetFont(const Font *font);
		virtual void SetText(u32 dictId);
		virtual void SetText(WideString str);
		virtual void SetText(const String &str);
		//virtual void Print(WideString str, ...);
		virtual void SetLabelBlending(eBlendMode blending);
		virtual void SetLabelColor(u8 r, u8 g, u8 b, u8 a);
		virtual void SetLabelColor(PIXEL px);
		virtual void SetLabelPressColor(u8 r, u8 g, u8 b, u8 a);
		virtual void SetLabelPressColor(PIXEL px);
		virtual void SetLabelHoverColor(u8 r, u8 g, u8 b, u8 a);
		virtual void SetLabelHoverColor(PIXEL px);
		virtual void SetLabelDisabledColor(u8 r, u8 g, u8 b, u8 a);
		virtual void SetLabelDisabledColor(PIXEL px);
		virtual void SetLabelPressOffset(f32 x, f32 y);
		virtual void SetLabelHoverOffset(f32 x, f32 y);

		// ITransformable2D - Sprite
		virtual void SetSpriteWidth(f32 w);
		virtual void SetSpriteHeight(f32 h);

		virtual void SetSpriteX(f32 x);
		virtual void SetSpriteY(f32 y);
		virtual void AddSpriteX(f32 value);
		virtual void AddSpriteY(f32 value);

		virtual void SetSpritePosition(f32 x, f32 y);
		virtual void AddSpritePosition(f32 x, f32 y);

		virtual void SetSpriteRotation(f32 rot);
		virtual void AddSpriteRotation(f32 rot);

		virtual void SetSpriteScaleX(f32 scaleX);
		virtual void SetSpriteScaleY(f32 scaleY);
		virtual void SetSpriteScale(f32 scale);
		virtual void SetSpriteScale(f32 scaleX, f32 scaleY);

		virtual void AddSpriteScaleX(f32 scaleX);
		virtual void AddSpriteScaleY(f32 scaleY);
		virtual void AddSpriteScale(f32 scale);
		virtual void AddSpriteScale(f32 scaleX, f32 scaleY);

		virtual f32 GetSpriteWidth() const;
		virtual f32 GetSpriteHeight() const;

		virtual f32 GetSpriteX() const;
		virtual f32 GetSpriteY() const;

		virtual f32 GetSpriteRotation() const;

		virtual f32 GetSpriteScaleX() const;
		virtual f32 GetSpriteScaleY() const;

		virtual const char *GetSpriteFilename() const;
		virtual IImage *GetSpriteTexture() const;

		virtual void SetSpriteAutoUpdate(BOOL b);
		virtual BOOL IsSpriteAutoUpdate() const;

		// Sprite
		virtual void SetSpriteBlending(Seed::IRenderable::eBlendMode op);
		virtual void SetSpriteColor(u8 r, u8 g, u8 b, u8 a);
		virtual void SetSpriteColor(PIXEL px);
		virtual PIXEL GetSpriteColor();
		virtual void SetSpritePressOffset(f32 x, f32 y);
		virtual void SetSpriteHoverOffset(f32 x, f32 y);
		//virtual u8 GetSpritePixelAlpha(u32 x, u32 y) const;

		// IWidget
		virtual void SetDisabled(BOOL b);

		// IRenderable
		virtual void Render(f32 delta);

		// IResource
		virtual BOOL Load(const char *filename, ResourceManager *res = &glResourceManager, IMemoryPool *pool = pDefaultPool);
		virtual BOOL Unload();

		// IObject
		virtual const char *GetObjectName() const;
		virtual int GetObjectType() const;

		virtual BOOL ContainsPoint(f32 x, f32 y);

	protected:
		void *operator new(size_t len);
		void operator delete(void *ptr);
		void *operator new [](size_t);
		void operator delete [] (void *);

	protected:
		Sprite	cSprite;
		Label	cLabel;

	private:
		SEED_DISABLE_COPY(Button);

		BOOL CheckPixel(f32 x, f32 y);
		void UpdateLabel();
		void UpdateSprite();

	private:
		const char	*pPreviousEnabledAnimation;

		eCollisionType eButtonCollision;
		BOOL		bDisableHover;
		BOOL		bFrameControl;
		BOOL		bSpriteBased;
		BOOL		bLabelBased;
		BOOL		bButtonChanged;
		BOOL		bSpriteAutoUpdate;
		BOOL		bCenterDrag;
		u32			iDraggingPriority;
		u32			iOldPriority;
		PIXEL		iLabelPressColor;
		PIXEL		iLabelHoverColor;
		PIXEL		iLabelDisabledColor;
		PIXEL		iLabelColor;
		eBlendMode	eLabelBlendOperation;
		/*f32		fOriginalDraggableX;
		f32			fOriginalDraggableY;*/
		f32			fLabelOffsetX;
		f32			fLabelOffsetY;
		f32			fLabelPressOffsetX;
		f32			fLabelPressOffsetY;
		f32			fLabelHoverOffsetX;
		f32			fLabelHoverOffsetY;
		f32			fSpriteOffsetX;
		f32			fSpriteOffsetY;
		f32			fSpritePressOffsetX;
		f32			fSpritePressOffsetY;
		f32			fSpriteHoverOffsetX;
		f32			fSpriteHoverOffsetY;
		f32			fDragOffsetX;
		f32			fDragOffsetY;
		eHorizontalAlignment	eLabelHAlignment;
		eVerticalAlignment		eLabelVAlignment;
		const char  *pstrSpriteFilename;

		File			stFile;
		CollisionMask	*pMask;
};


} // namespace


#endif // __GUI_BUTTON_H__
