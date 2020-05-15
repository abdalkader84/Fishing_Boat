//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Objects.hpp>
#include <FMX.Types.hpp>
#include <FMX.Layouts.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TPath *Path1;
	TImage *Image1;
	TRectangle *stick;
	TTimer *game_engin;
	TRectangle *Rectangle1;
	TPath *Path2;
	TRectangle *tm;
	TRectangle *tar1;
	TTimer *Timer1;
	TRectangle *tar2;
	TImage *Image2;
	TPath *Path3;
	TLayout *Layout1;
	TCircle *Circle1;
	TCircle *Circle2;
	TCircle *Circle3;
	TCircle *Circle4;
	TCircle *Circle5;
	TCircle *Circle6;
	TText *Text1;
	void __fastcall game_enginTimer(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          float X, float Y);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift, float X, float Y);

private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
	void __fastcall check_boundary( );
	void __fastcall set_level(TPath * pth, int lvl,int p);
	void __fastcall create_target( );
	void __fastcall move_target( );
	void __fastcall catch_target( );
	void __fastcall move_target_catch( );
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
