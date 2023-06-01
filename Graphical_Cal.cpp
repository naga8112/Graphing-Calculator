#include<fstream.h>
#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<math.h>
#include<ctype.h>
#include<string.h>
#include<process.h>
#include<dos.h>
#include<stdlib.h>
int cx,cy,q;
class mouse
{
		REGS regs;
		int no_buttons;
	public:
		void show_mouse();
		void get_status();
		void hide_mouse();
		mouse()
		{
			regs.x.ax=0;
			int86(0x33,&regs,&regs);
			if(regs.x.ax==0xfff)no_buttons=regs.x.bx;
			//cout<<"Error initializing mouse";
		}
}m;
		void mouse::show_mouse()
		{
			regs.x.ax=0x01;
			int86(0x33,&regs,&regs);
		}
		void mouse::hide_mouse()
		{
			regs.x.ax=0x02;
			int86(0x33,&regs,&regs);
		}
		void mouse::get_status()
		{
			regs.x.ax=0x03;
			int86(0x33,&regs,&regs);
			while(q!=1)
			{
				regs.x.ax=0x03;
				int86(0x33,&regs,&regs);
				q=regs.x.bx;
				cx=regs.x.cx;
				cy=regs.x.dx;
			}
			delay(600);
			q=0;
		}
double scalex=20.0,scaley=20.0;
int cstclr=1,a=9,b=2,j,bk=15,pat[20]={1,2,4,9,12,0},clr=-1,eqn=1,pre=1,n;
char choice='p',brackets[20];
double evaluate(char g[],double n)
{
	double st[50]={0.0},stop=0.0;
	int temp;
	char trig[50];
	for(int i=0,j=-1,k=-1;((g[i]!='\0')&&(stop==0));i++)
	{
		if(g[i]=='*')
		{
			st[j-1]=st[j-1]*st[j];
			j--;
		}
		else if(g[i]=='/')
		{
			if(st[j]!=0)st[j-1]=st[j-1]/st[j];
			else stop++;
			j--;
		}
		else if(g[i]=='+')
		{
			st[j-1]=st[j-1]+st[j];
			j--;
		}
		else if(g[i]=='-')
		{
			st[j-1]=st[j-1]-st[j];
			j--;
		}
		else if(g[i]=='^')
		{
			if(st[j-1]>=0) st[j-1]=pow(st[j-1],st[j]);
			else
			{
				double temp=st[j-1];
				st[j-1]=pow(st[j-1],floor(st[j]));
				st[j]=st[j]-floor(st[j]);
				if(st[j]!=0)
				{
					if(int(ceil(1/st[j]))%2!=0)st[j-1]*=-1*pow(fabs(temp),st[j]);
					else stop++;
				}
			}
			/*if(int(st[j])==st[j])st[j-1]=pow(st[j-1],st[j]);
			else if(st[j-1]>=0)st[j-1]=pow(st[j-1],st[j]);
			else if(int(ceil(1/st[j]))%2!=0)st[j-1]=-1*pow(fabs(st[j-1]),st[j]);
			else stop++; */
			j--;
		}
		else if(g[i]=='(')
		{
			i++;
			j++;
			double s=0,flag=0,decimal=1;
			while(isdigit(g[i])||(g[i]=='.'))
			{
				if(flag!=0)decimal*=10;
				if(g[i]!='.')s=s*10+float(g[i])-48;
				else flag++;
				i++;
			}
			st[j]=s/decimal;
		}
		else if(g[i]=='e')
		{
			j++;
			st[j]=2.718;
		}
		else if(isupper(g[i]))
		{
			k++;
			trig[k]=g[i];
			i++;
		}
		else if(g[i]==')')
		{
			if(trig[k]=='A')
			{
				st[j]=tan(st[j]);
				k--;
			}
			else if(trig[k]=='B')
			{
				st[j]=1/tan(st[j]);
				k--;
			}
			else if(trig[k]=='C')
			{
				st[j]=1/sin(st[j]);
				k--;
			}
			else if(trig[k]=='D')
			{
				st[j]=1/cos(st[j]);
				k--;
			}
			else if(trig[k]=='E')
			{
				if(st[j]>0)st[j]=log(st[j]);
				else stop++;
				k--;
			}
			else if(trig[k]=='F')
			{
				st[j]=sin(st[j]);
				k--;
			}
			else if(trig[k]=='G')
			{
				st[j]=cos(st[j]);
				k--;
			}
			else if(trig[k]=='H')
			{
			       if(st[j]<0)  st[j]-=(int)st[j]-1;
			       else st[j]-=(int)st[j];
			       k--;
			}
			else if(trig[k]=='I')
			{
				st[j]=floor(st[j]);
				k--;
			}
			else if(trig[k]=='J')
			{
			    st[j]=fabs(st[j]);
			    k--;
			}
		}
		else
		{
			j++;
			st[j]=n;
		}
	}
	if(stop==0)return st[0];
	else return 30;
}
int post_fix(char g[],int p)
{
	char re[50]=" ",s[50],ch;
	if(eqn==1)ch='x';
	else ch='y';
	int k=0,l=0,m=-1,times=1,trig=0,error=0,terms[20]={0,0,0,0,0};
	s[0]='(';
	s[1]='(';
	for(int i=0,j=2;((g[i]!='\0')&&(error==0));i++)
	{
	     if((g[i]=='(')||(g[i]=='{')||(g[i]=='['))
	     {
		m++;
		if(g[i]=='(')brackets[m]=')';
		else if(g[i]=='[')brackets[m]=']';
		else brackets[m]='}';
		terms[l]++;
		l++;
	     }
	     if((g[i]=='(')||(g[i]=='^'))
	     {
		if((s[j-1]=='^')&&(g[i]!='('))
		{
			re[k]=s[j-1];
			j--;
			k++;
		}
		if(g[i]=='^')terms[l]--;
		s[j]=g[i];
		j++;
	     }
	     else if((g[i]=='+')||(g[i]=='-'))
	     {
		if((isupper(re[k-2]))||(g[i-1]=='('))
		{
		     re[k]='(';
		     re[k+1]='0';
		     re[k+2]=')';
		     k+=3;
		}
		else terms[l]--;
		while((s[j-1]!='(')&&(j>=0))
		{
			re[k]=s[j-1];
			j--;
			k++;
		}
		s[j]=g[i];
		j++;
	     }
	     else if((g[i]=='*')||(g[i]=='/'))
	     {
		terms[l]--;
		while((s[j-1]!='(')&&(s[j-1]!='+')&&(s[j-1]!='-'))
		{
			re[k]=s[j-1];
			j--;
			k++;
		}
		s[j]=g[i];
		j++;
	     }
	     else if((g[i]==')')||(g[i]==']')||(g[i]=='}'))
	     {
		if((l!=-1)&&(terms[l]!=1))error++;
		terms[l]=0;l--;
		if((m!=-1)&&(g[i]!=brackets[m])) error++;
		m--;
		while((s[j-1]!='(')&&(j>=0))
		{
			re[k]=s[j-1];
			j--;
			k++;
		}
		if(trig>0)
		{
			re[k]=')';
			k++;
			trig--;
		}
		j--;
	     }
	     else if(isdigit(g[i]))
	     {
		terms[l]++;
		re[k]='(';
		k++;
		do
		{
			re[k]=g[i];
			i++;
			k++;
		}while((isdigit(g[i]))||(g[i]=='.'));
		re[k]=')';
		k++;
		i--;
	     }
	     else if(g[i]=='|')
	     {
		if(times%2==0)
		{
		    if(brackets[m]!='|')error++;
		    m--;
		    if(terms[l]!=1)error++;
		    terms[l]=0;l--;
		    while((s[j-1]!='(')&&(j>=0))
		    {
			re[k]=s[j-1];
			j--;
			k++;
		    }
		    re[k]=')';
		    k++;
		    j--;
		}
		else
		{
			m++;
			brackets[m]='|';
			terms[l]++;
			l++;
			re[k]='J';
			k++;
			re[k]='(';
			k++;
			s[j]='(';
			j++;
		}
		times++;
	     }
	     else if(g[i]=='[')
	     {
			re[k]='I';
			k++;
			re[k]='(';
			k++;
			s[j]='(';
			j++;
			trig++;
	     }
	     else if(g[i]=='{')
	     {
			re[k]='H';
			k++;
			re[k]='(';
			k++;
			s[j]='(';
			j++;
			trig++;
	     }
	     else if((g[i]=='l')&&(g[i+1]=='o')&&(g[i+2]=='g'))
	     {
			i+=3;
			if(g[i]!='(')error++;
			else
			{
				m++;
				brackets[m]=')';
				terms[l]++;
				l++;
			}
			re[k]='E';
			k++;
			re[k]='(';
			k++;
			s[j]='(';
			j++;
			trig++;
	     }
	     else if((g[i]=='t')&&(g[i+1]=='a')&&(g[i+2]=='n'))
	     {
			i+=3;
			if(g[i]!='(')error++;
			else
			{
				m++;
				brackets[m]=')';
				terms[l]++;
				l++;
			}
			re[k]='A';
			k++;
			re[k]='(';
			k++;
			s[j]='(';
			j++;
			trig++;
	     }
	     else if((g[i]=='c')&&(g[i+1]=='o')&&(g[i+2]=='t'))
	     {
			i+=3;
			if(g[i]!='(')error++;
			else
			{
				m++;
				brackets[m]=')';
				terms[l]++;
				l++;
			}
			re[k]='B';
			k++;
			re[k]='(';
			k++;
			s[j]='(';
			j++;
			trig++;
	     }
	     else if((g[i]=='c')&&(g[i+1]=='o')&&(g[i+2]=='s')&&(g[i+3]=='e')&&(g[i+4]=='c'))
	     {
			i+=5;
			if(g[i]!='(')error++;
			else
			{
				m++;
				brackets[m]=')';
				terms[l]++;
				l++;
			}
			re[k]='C';
			k++;
			re[k]='(';
			k++;
			s[j]='(';
			j++;
			trig++;
	     }
	     else if((g[i]=='s')&&(g[i+1]=='e')&&(g[i+2]=='c'))
	     {
			i+=3;
			if(g[i]!='(')error++;
			else
			{
				m++;
				brackets[m]=')';
				terms[l]++;
				l++;
			}
			re[k]='D';
			k++;
			re[k]='(';
			k++;
			s[j]='(';
			j++;
			trig++;
	     }
	     else if((g[i]=='s')&&(g[i+1]=='i')&&(g[i+2]=='n'))
	     {
			i+=3;
			if(g[i]!='(')error++;
			else
			{
				m++;
				brackets[m]=')';
				terms[l]++;
				l++;
			}
			re[k]='F';
			k++;
			re[k]='(';
			k++;
			s[j]='(';
			j++;
			trig++;
	     }
	     else if((g[i]=='c')&&(g[i+1]=='o')&&(g[i+2]=='s'))
	     {
			i+=3;
			if(g[i]!='(')error++;
			else
			{
				m++;
				brackets[m]=')';
				terms[l]++;
				l++;
			}
			re[k]='G';
			k++;
			re[k]='(';
			k++;
			s[j]='(';
			j++;
			trig++;
	     }
	     else if((g[i]=='e')||(g[i]==ch))
	     {
		terms[l]++;
		re[k]=g[i];
		k++;
	     }
	     else error++;
	     if((terms[l]!=0)&&(terms[l]!=1)) error++;
	}
	while((s[j-1]!='(')&&(j>=0))
	{
		if((l!=-1)&&(terms[l]!=1))error++;
		l--;
		re[k]=s[j-1];
		j--;
		k++;
	}
	if(m!=-1)	error++;

	if(error)return 0;
	strcat(re,"(1)(");
	k+=4;
	re[k]=p+48;
	k++;
	strcat(re,")/^");
	k+=3;
	while((s[j-1]!='(')&&(j>=0))
	{
		re[k]=s[j-1];
		j--;
		k++;
	}
	strcpy(g,re);
	return 1;
}
void heading()
{
	setfillstyle(1,14);
	bar(481,0,640,480);
	settextstyle(7,0,2);
	setcolor(9);
	outtextxy(490,5,"Graph Plotter");
	line(481,35,640,35);
	settextstyle(0,0,1);
}
void clear_graph()
{
	setfillstyle(1,bk);
	bar(0,0,640,480);
	setcolor(11);
	for(int j=scalex;j+240<=480;j+=scalex)
	{
		line(240+j,0,240+j,480);
		line(240-j,0,240-j,480);
	}
	for(j=scaley;j+240<=480;j+=scaley)
	{
		line(0,240+j,480,240+j);
		line(0,240-j,480,240-j);
	}
	setcolor(BROWN);
	line(240,0,240,480);
	line(0,240,480,240);
	clr=-1;
}
void ftoa(float f,char x[20])
{
	if(f<0)x[0]='-';
	char s1[20];
	itoa(abs(f),s1,10);
	strcat(x,s1);
	long h=(fabs(f)-abs(f))*10000;
	ltoa(h,s1,10);
	strcat(x,".");
	strcat(x,s1);
}
int grinput(char s[],int x,int y)
{
	char c[2]="\0\0";
	int t=0,i=x,j=y,err=0,dec=0;
	do
	{
		c[0]=getch();
		if((isdigit(c[0])==0)&&(c[0]!='.')&&(c[0]!=8)&&(c[0]!=13))err++;
		else if(c[0]=='.')
		{
			if(dec==1)err++;
			else dec++;
		}
		if(err==0)
		{
			if(c[0]==8)
			{
				i-=8;
				t--;
				bar(i,j,640,j+10);
			}
			else if(c[0]!=13)
			{
				s[t]=c[0];
				outtextxy(i,j,c);
				i+=8;
				t++;
			}
		}
	}while((c[0]!=13)&&(t<10)&&(err==0));
	s[t]='\0';
	if(err)return 0;
	return 1;
}
void scale()
{
	clear_graph();
	char s[20]="\0\0\0",h[20]="\0\0\0",xx[20],yy[20];
	ftoa(scalex,h);strcat(h," pixels");
	ftoa(scaley,s);strcat(s," pixels");
	int errx=1,erry=1;
	do
	{
		errx=0,erry=0;
		heading();
		outtextxy(520,42,"New scale");
		outtextxy(481,314,"Current scale ");
		outtextxy(481,331,"1 unit in x axis :");
		outtextxy(488,345,h);
		outtextxy(481,365,"1 unit in y axis :");
		outtextxy(488,379,s);
		outtextxy(481,120,"From 20 to 60 pixels");
		outtextxy(481,55,"1 unit in x axis :");
		errx=grinput(xx,498,70);
		if(errx==1)
		{
			outtextxy(481,83,"1 unit in y axis :");
			erry=grinput(yy,498,95);
		}
	}while((errx==0)||(erry==0)||(atof(xx)<20)||(atof(xx)>60)||(atof(yy)<20)||(atof(yy)>60));//(x<20)||(x>60)||(y<20)||(y>60));
	scalex=atof(xx);
	scaley=atof(yy);
	clear_graph();
}
void input(char st[])
{
		char ch[2]="\0\0";
			int t=0,j=261,err=0,r=481;
			do
			{
				ch[0]=getch();
				if((isdigit(ch[0])==0)&&(ch[0]!=8)&&(ch[0]!=13))err++;
				if(err==0)
				{
					if(ch[0]==8)
					{
						r-=8;
						t--;
						bar(r,j,640,j+10);
					}
					else if(ch[0]!=13)
					{
						st[t]=ch[0];
						outtextxy(r,j,ch);
						r+=8;
						t++;
					}
				}
				else
				{
					setfillstyle(1,14);
					bar(481,261,640,273);
					r=481;
					t=0;
					err=0;
				}
			}while((ch[0]!=13)&&(t<3));
			st[t]='\0';
}
void set_pattern()
{
      int c,k=0;
      heading();
      outtextxy(520,42,"Set pattern");
      outtextxy(481,275,"Colours available :");
      setfillstyle(1,bk);
      bar(481,287,640,477);
      char s[4]="01.";
      for(int i=1;i<15;i++)
      {
		setfillstyle(1,i);
		outtextxy(481,287+12*i,s);
		bar(505,287+12*i,630,297+12*i);
		if(s[1]=='9')
		{
			s[1]='0';
			s[0]='1';
		}
		else s[1]++;
      }
      strcpy(s,"00.");
      outtextxy(481,55,"Gno  Graph-colour");
      setfillstyle(1,bk);
      bar(481,67,640,247);
      do
      {
		do
		{
			setfillstyle(1,14);
			bar(481,249,640,274);
			if(k!=0)
			{
				setfillstyle(1,pat[k-1]);
				outtextxy(481,67+12*k,s);
				bar(505,67+12*k,630,75+12*k);
			}
			if(k>0)outtextxy(481,249,"Your No(0 to Exit):");
			else outtextxy(481,249,"Your No : -");
			char st[20];
			input(st);
			c=atoi(st);
			if((c==0)&&(k==0))c=15;
		}while((c<0)||(c>14));
		if(s[1]=='9')
		{
			s[1]='0';
			s[0]='1';
		}
		else s[1]++;
		pat[k]=c;
		k++;
	}while((pat[k-1]!=0)&&(k<14));
	if(k==14)
	{
		setfillstyle(1,pat[k-1]);
		outtextxy(481,67+12*k,s);
		bar(505,67+12*k,630,75+12*k);
		setfillstyle(1,14);
		bar(481,249,640,274);
		outtextxy(481,249,"Hit any key");
		getch();
	}
	choice='p';
}
void const_colour()
{
	int c;
	do
	{
		heading();
		outtextxy(500,42,"Constant colour");
		outtextxy(481,400,"Current colour ");
		setfillstyle(1,cstclr);
		rectangle(499,418,631,430);
		bar(500,419,630,429);
		outtextxy(481,55,"Colours available :");
		setfillstyle(1,bk);
		bar(481,67,640,247);
		char s[4]="01.";
		for(int i=1;i<15;i++)
		{
			setfillstyle(1,i);
			outtextxy(481,67+12*i,s);
			bar(505,67+12*i,630,75+12*i);
			if(s[1]=='9')
			{
				s[1]='0';
				s[0]='1';
			}
			else s[1]++;
		}
		outtextxy(481,69+12*i,"Enter your Number : ");
		char st[20];
		input(st);
		c=atoi(st);
	}while((c<1)||(c>14));
	cstclr=c;
	choice='c';
}
void colour_menu()
{
	int c,ch2,stop=1;
	do
	{
		do
		{
			heading();
			outtextxy(520,55,"Graph colour");
			outtextxy(490,79,"1. Constant colour");
			outtextxy(490,96,"2. Set pattern");
			outtextxy(490,113,"3. Back");
			outtextxy(490,130,"Your option :");
			c=getch()-48;
			char st[3]="\0\0\0";st[0]=c+48;
			outtextxy(600,130,st);
			ch2=getch();
		}while((c<1)||(c>3)||(ch2!=13));
		if(c==1)const_colour();
		else if(c==2)set_pattern();
		else stop--;
	}while(stop);
}
void plot_graph()
{
	char ch,g[50];
	int p=0;
	do
	{
		if(pre)
		{
			if(choice=='p')
			{
				clr++;
				if(pat[clr]==0)clr=0;
				n=pat[clr];
			}
			else n=cstclr;
		}
		 char ch2;
		 heading();
		 char s[20],x[20]="\0\0\0\0\0",h[20]="\0\0\0\0\0";
		 ftoa(scalex,h);strcat(h," pixels");
		 ftoa(scaley,x);strcat(x," pixels");
		 outtextxy(481,314,"Current scale ");
		 outtextxy(481,331,"1 unit in x axis :");
		 outtextxy(488,345,h);
		 outtextxy(481,365,"1 unit in y axis :");
		 outtextxy(488,379,x);
		 outtextxy(481,400,"Current colour ");
		 setfillstyle(1,n);
		 rectangle(499,418,631,430);
		 bar(500,419,630,429);
		 setfillstyle(1,14);
		 outtextxy(481,55,"ENTER THE POWER OF");
		 if(eqn==1)outtextxy(481,67,"Y : ");
		 else outtextxy(481,67,"X : ");
		 do
		 {
			bar(507,67,640,79);
			p=getch()-48;
			char st[3]="\0\0\0";st[0]=p+48;
			outtextxy(508,67,st);
			ch2=getch();
		}while((p<=0)||(ch2!=13));
		outtextxy(481,79,"ENTER THE EQUATION :");
		char c[2]="\0\0";int i=481,j=100,k=0;
		do
		{
		  c[0]=getch();
		  if(c[0]==8)
		  {
			if(i!=481)i-=8;
			else
			{
				i=629;
				j-=12;
			}
			bar(i,j,640,j+12);
			k--;
		  }
		  else if(c[0]!=13)
		  {
			g[k]=c[0];
			outtextxy(i,j,c);
			if((k!=0)&&(k%18==0))
			{
			     j+=12;
			     i=481;
			}
			else i+=8;
			k++;
		  }
		}while(c[0]!=13);
		g[k]='\0';
		if(post_fix(g,p))
		{
			pre=1;
			if(eqn==1)
			{
				for(double x=-240/scalex;x<=240/scalex;x+=0.005/scalex)
				{
					double y=evaluate(g,x);
					if((y<240/scaley)&&(y>-240/scaley))
					{
						putpixel(x*scalex+240,240-(y*scaley),n);
						if(p%2==0) putpixel(x*scalex+240,240+y*scaley,n);
					}
				}
			}
			else
			{
				for(double y=-240/scaley;y<=240/scaley;y+=0.005/scaley)
				{
					double x=evaluate(g,y);
					if((x<240/scalex)&&(x>-240/scalex))
					{
						putpixel(x*scalex+240,240-(y*scaley),n);
						if(p%2==0) putpixel(-x*scalex+240,240-y*scaley,n);
					}
				}
			}
		}
		else
		{
			outtextxy(481,219,"Invalid Expression");
			pre=0;
		}
		outtextxy(481,119,"Hit any key");
		outtextxy(481,131,"(x to Exit)");
		ch=getch();
	}while(ch!='x');
}
void bkcolor()
{
	int c,ch2;
	do
	{
		heading();
		outtextxy(520,42,"Background");
		outtextxy(481,55,"Colours available :");
		setfillstyle(1,0);
		outtextxy(481,67,"1.");
		bar(505,67,630,75);
		setfillstyle(1,15);
		outtextxy(481,79,"2.");bar(505,79,630,87);
		outtextxy(490,105,"Your Option : ");
		c=getch()-48;
		char st[3]="\0\0\0";st[0]=c+48;
		outtextxy(600,105,st);
		ch2=getch();
	}while((c<1)||(c>2)||(ch2!=13));
       if(c==1)	bk=0;
       else bk=15;
	clear_graph();
}
void equation_type()
{
	int c,ch2;
	do
	{
		heading();
		outtextxy(481,55,"Equation types :");
		outtextxy(481,73,"1. y in terms of x");
		outtextxy(481,90,"2. x in terms of y");
		outtextxy(490,107,"Your option :");
		c=getch()-48;
		char st[3]="\0\0\0";st[0]=c+48;
		outtextxy(600,107,st);
		ch2=getch();
	}while((c<1)||(c>2)||(ch2!=13));
	eqn=c;
}
void settings()
{
		int ch,stop=1,ch2;
		do
		{
			do
			{
				heading();
				outtextxy(540,55,"Settings");
				outtextxy(490,79,"1. Scale");
				outtextxy(490,96,"2. Graph Colour");
				outtextxy(490,113,"3. Background");
				outtextxy(490,130,"4. Equations");
				outtextxy(490,147,"5. Back");
				outtextxy(490,164,"Your option :");
				ch=getch()-48;
				char st[3]="\0\0\0";st[0]=ch+48;
				outtextxy(600,164,st);
				ch2=getch();
			}while((ch<1)||(ch>5)||(ch2!=13));
			if(ch==1)scale();
			else if(ch==2)colour_menu();
			else if(ch==3)bkcolor();
			else if(ch==4)equation_type();
			else stop--;
		}while(stop);
}
void coordinates()
{
	char c='x';
	do
	{
		heading();
		outtextxy(481,55,"Click on the graph");
		outtextxy(481,72,"To get coordinates");
		m.show_mouse();
		m.get_status();
		m.hide_mouse();
		gotoxy(65,6);
		char p[20]="\0\0\0",s[20]="\0\0\0",h[20]="\0\0\0";
		ftoa((cx-240)/scalex,h);
		ftoa((240-cy)/scaley,s);
		p[0]='(';
		if(cx<=480)
		{
			strcat(p,h);
			strcat(p,",");
			strcat(p,s);
			strcat(p,")");
			outtextxy(481,89,p);
		}
		else outtextxy(481,89,"Invalid click");
		outtextxy(481,119,"Hit any key");
		outtextxy(481,131,"(x to Exit)");
		c=getch();
	}while(c!='x');
}
void Graphcal()
{
	initgraph(&a,&b,"c:\\turboc3\\bgi");
	clear_graph();
	int ch;
	do
	{
		int ch2;
		do
		{
			heading();
			outtextxy(540,55,"Menu");
			outtextxy(490,79,"1. Plot graph");
			outtextxy(490,96,"2. Settings");
			outtextxy(490,113,"3. Clear screen");
			outtextxy(490,130,"4. Coordinates");
			outtextxy(490,147,"5. Exit");
			outtextxy(490,164,"Your option :");
			ch=getch()-48;
			char st[3]="\0\0\0";st[0]=ch+48;
			outtextxy(600,164,st);
			ch2=getch();
		}while((ch<1)||(ch>5)||(ch2!=13));
		if(ch==1)plot_graph();
		else if(ch==2)settings();
		else if(ch==3)clear_graph();
		else if(ch==4)coordinates();
	}while(ch!=5);
}
void main(){
	Graphcal();
}