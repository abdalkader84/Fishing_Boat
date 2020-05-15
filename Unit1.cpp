//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm1 *Form1;
unsigned long tim;
int balance;

unsigned long target_tim=0;
unsigned long max_target_tim=40;

int tar_tp=1;
int sped_max=2;
int rot_max=40;
int rot=5;
float sped;
float Hs=15;
float Vs=20;
float G=0.05;
int  level;
int phs;
bool stick_move_dn=false;
bool stick_move_up=false;
bool cath=false;
float msx;
TRectangle * r_catch;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	srand(time(NULL));
}
//---------------------------------------------------------------------------
void __fastcall TForm1::check_boundary( )
{
	if (abs(balance)>rot_max) {
	   balance=rot_max*(abs(balance)/balance);
	}
	if (abs(sped)>sped_max) {
	   sped=sped_max*(abs(sped)/sped);
	}

	if (stick->Height<=40) {
		stick->Height=40;
	}
	if (stick->Height>=650) {
		stick->Height=650;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::set_level(TPath * pth, int lvl,int p)
{
	pth->Data->Clear();
	pth->Data->MoveTo(TPointF(0,pth->Height));
	pth->Data->LineTo(TPointF(0,0));
	pth->Data->LineTo(TPointF(0,lvl));
	//-------------------------------------------
	for (int i=0; i <(int) pth->Width; i++) {
		pth->Data->LineTo(TPointF(i,lvl+5*sin(3*i*3.1415/180 + p*phs*3.1415/180)));
	}
	//-------------------------------------------
	pth->Data->LineTo(TPointF(pth->Width,lvl));
	pth->Data->LineTo(TPointF(pth->Width,0));
	pth->Data->LineTo(TPointF(pth->Width,pth->Height));
	pth->Data->ClosePath();
	phs+=10;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::create_target( )
{

    if (tim-target_tim>=max_target_tim)
	{
		TRectangle * rc=NULL;
		if (tar_tp==1) 
		{			
			rc=(TRectangle *) (tar1->Clone(Form1));
			rc->Position->X=-rc->Width;
			rc->Position->Y=RandomRange(Path2->Height/2 + 50 ,Path2->Height );
			rc->Visible=true;
			rc->Tag=1;
			tar_tp=2;
		}
		else if (tar_tp==2)
		{
			rc=(TRectangle *) (tar2->Clone(Form1));
			rc->Position->X=Path2->Width+ rc->Width;
			rc->Position->Y=RandomRange(Path2->Height/2 + 50 ,Path2->Height );
			rc->Visible=true;
			rc->Tag=2;
			tar_tp=1;
		}
		Form1->AddObject(rc);
		Image1->BringToFront();
		Path2->BringToFront();
		target_tim=tim;
	}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::move_target( )
{
	for (int   i = 0; i < Form1->Children->Count; i++) {
		TRectangle * r=(TRectangle *)Form1->Children->Items[i];
		if (r==r_catch) {continue;}
		if ((r->ClassName()=="TRectangle") && (r->Tag==1)) {
			r->Position->X=r->Position->X+9;
		}
		if ((r->ClassName()=="TRectangle") && (r->Tag==2)) {
			r->Position->X=r->Position->X-9;
		}
	}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::catch_target( )
{
	for (int   i = 0; i < Form1->Children->Count; i++)
	{
		TRectangle * r=(TRectangle *)Form1->Children->Items[i];
		if (r==tar1 || r==tar2) {continue;}
		if ((r->ClassName()=="TRectangle") && ((r->Tag==1)||(r->Tag==2)))
		{
			if (cath==false)
			{
				if (tm->AbsoluteRect.Intersects(r->AbsoluteRect)==true)
				{
					//game_engin->Enabled=false;
					stick_move_up=true;
					stick_move_dn=false;
					cath=true;
					r_catch=r;
					r_catch->TagFloat=-15; 
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
TRectF get_rect(TImage * rc)
{
	TRectF r;
	r.Top=rc->Position->Y+rc->Height/4;
	r.left=rc->Position->X+rc->Width/4;
	r.right=rc->Position->X+rc->Width-rc->Width/4;
	r.Bottom=rc->Position->Y+0.75*rc->Height;
	return r;
}
void __fastcall TForm1::move_target_catch( )
{
	r_catch->RotationCenter->Point=TPointF(0.5,0.5);
	r_catch->RotationAngle=r_catch->RotationAngle+30;
	r_catch->TagFloat+=3;
	r_catch->Position->Y=r_catch->Position->Y+r_catch->TagFloat;
	TRectF r=get_rect(Image1);

	if (r.Contains(r_catch->AbsoluteRect.CenterPoint())==true)
	{
		Form1->RemoveObject(r_catch);
		Image1->AddObject(r_catch);                                
		r_catch->Position->X=r_catch->Position->X-Image1->Position->X;
		r_catch->Position->Y=r_catch->Position->Y-Image1->Position->Y;
		r_catch->Tag=0;
		r_catch=NULL;
		cath=false;
		
		for (int j=0; j<Image1->Children->Count; j++)
		{
			if (Image1->Children->Items[j]->ClassName()=="TImage") {
			   Image1->Children->Items[j]->BringToFront();	
               break; 
			}
		}	
	} else
	if (r_catch->AbsoluteRect.CenterPoint().Y>level+50)
	{
		if (r_catch->Tag==1)
		{
			r_catch->RotationCenter->Point=TPointF(1,0.5);
		}
		if (r_catch->Tag==2)
		{
			r_catch->RotationCenter->Point=TPointF(0,0.5);
		}

		r_catch->RotationAngle=0;
		r_catch=NULL;
		cath=false;
	}

}
//---------------------------------------------------------------------------
void __fastcall TForm1::game_enginTimer(TObject *Sender)
{
	tim++;
	
	create_target();
	move_target();
	catch_target();

	if (GetKeyState(VK_LEFT)& 0x8000)
	{
		balance-=rot;
	}
	else if (GetKeyState(VK_RIGHT)& 0x8000)
	{
		balance+=rot;
	}
	else if (GetKeyState(VK_UP)& 0x8000) {
		sped-=0.1;
	}
	else if (GetKeyState(VK_DOWN)& 0x8000) {
		sped+=0.1;
	}
	else
	{
	   sped+=0.1;
	   balance+=rot/1 *-sin(balance *3.1415/180);

	}

	if (GetKeyState(VK_SPACE)& 0x8000) {

	}

	if (false) // true for mouse movement
	{
		// with mouse movement

		float f=0;
		f=msx-(Image1->Position->X+Image1->Width*0.5);
		f= 5*f/10;
		balance=(int)f;

	}

	if (stick_move_dn==true) {
	   stick->Height=stick->Height+7;
	}
	if (stick_move_up==true) {
		if (cath==true)
		{
			stick->Height=stick->Height-14;
		}
		else
		{
            stick->Height=stick->Height-7;
		}

	}


	check_boundary( );

	Image1->RotationAngle=balance/20;
	// for key board movement
	Image1->Position->X=Image1->Position->X-(-Hs)*5*sin(balance*3.1415/180) ;
	//-------------------------------
	// for mosuse movement
	//Image1->Position->X=msx - (Image1->Width*0.5);//-(-Hs)*5*sin(balance*3.1415/180) ;
	//-------------------------------
	Image1->Position->Y=Image1->Position->Y+2*sin(10*tim*3.1415/180) ;
	stick->RotationAngle=balance*40/(Min(200,(int) stick->Height));



	if (cath==true)
	{
		if (stick->Height<=40) 
		{
			move_target_catch();
		}
		else
		{
			if (r_catch->Tag==1)
			{
				r_catch->Position->Y=tm->AbsoluteRect.Top-35;
				r_catch->Position->X=tm->AbsoluteRect.Left-r_catch->Width;
				r_catch->RotationAngle=balance -90;
			}
			else
			{
				r_catch->Position->Y=tm->AbsoluteRect.Top-15;
				r_catch->Position->X=tm->AbsoluteRect.Left;
				r_catch->RotationAngle=balance +90;
			}
		}
	}


}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	level=Path1->Height/2+50;
	Layout1->Visible=true;
    Image1->Visible=true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          float X, float Y)
{
    if (Button==TMouseButton::mbLeft) {
		stick_move_dn=! stick_move_dn;
		stick_move_up=false;
	}
	else  if (Button==TMouseButton::mbRight) {
		stick_move_up=! stick_move_up;
		stick_move_dn=false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
	set_level(Path1,level,1);
	set_level(Path2,level+20,-1);
	set_level(Path3,level+8,-2);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormMouseMove(TObject *Sender, TShiftState Shift, float X,
          float Y)
{
	msx=X;
}
//---------------------------------------------------------------------------

