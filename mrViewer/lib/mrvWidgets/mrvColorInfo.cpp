/*
    mrViewer - the professional movie and flipbook playback
    Copyright (C) 2007-2022  Gonzalo Garramuño

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/**
 * @file   mrvColorInfo.cpp
 * @author gga
 * @date   Wed Nov 08 05:32:32 2006
 *
 * @brief  Color Info text display
 *
 *
 */


#include "mrvCore/mrvUtil.h"
#include "mrvCore/mrvI8N.h"
#include <string>
#include <sstream>
#include <limits>
#include <cmath>  // for std::isnan, std::isfinite

using namespace std;

#if defined(WIN32) || defined(WIN64)
# include <float.h>  // for isnan
# define isnan(x) _isnan(x)
# define isfinite(x) _finite(x)
#endif

#include <FL/Fl_Menu.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Box.H>
#include <FL/Enumerations.H>


#include "mrvCore/mrvColorSpaces.h"
#include "mrvCore/mrvString.h"
#include "mrViewer.h"
#include "mrvGL/mrvGLViewport.h"
#include "mrvColorInfo.h"

#include "mrvFl/mrvIO.h"

namespace
{
    const char* kModule = "cinfo";
}


namespace
{

void copy_color_cb( void*, mrv::Browser* w )
{
    if ( w->value() < 2 || w->value() > 11 )
        return;

    if ( w->text( w->value() ) == NULL ) return;

    size_t last;
    std::string line( w->text( w->value() ) );
    size_t start = line.find( '\t', 0 );
    line = line.substr( start + 1, line.size()-1 );
    while ( (start = line.find( '@', 0 )) != std::string::npos )
    {
        last = line.find( 'c', start );
        if ( last == std::string::npos ) break;

        if ( start > 0 )
            line = ( line.substr( 0, start-1) +
                     line.substr( last + 1, line.size()-1 ) );
        else
            line = line.substr( last + 1, line.size()-1 );
    }

    std::string copy = " ";
    last = 0;
    while ( (start = line.find_first_not_of( "\t ", last )) !=
            std::string::npos )
    {
        last = line.find_first_of( "\t ", start );
        if ( last == std::string::npos ) last = line.size();

        copy += line.substr( start, last-start ) + " ";
    }

    // Copy text to both the clipboard and to X's XA_PRIMARY
    Fl::copy( copy.c_str(), unsigned( copy.size() ), true );
    Fl::copy( copy.c_str(), unsigned( copy.size() ), false );
}

}

namespace mrv
{
    ViewerUI* ColorInfo::ui = NULL;



class ColorBrowser : public mrv::Browser
{
    int _value;
    ViewerUI*   ui;
public:
    ColorBrowser( int X, int Y, int W, int H, const char* L = 0 ) :
    mrv::Browser( X, Y, W, H, L ),
    _value( -1 )
    {
    }

    void main( ViewerUI* v ) {
        ui = v;
    }


    void resize( int X, int Y, int W, int H )
        {
            mrv::Browser::resize( X, Y, W, H );
            int* widths = column_widths();
            int WL, WH;
            fl_font( FL_HELVETICA, 14 );
            fl_measure( _("Maximum:"), WL, WH );
            WL += 8;
            int width = W - WL;
            int w5 = width / 4;
            widths[0] = WL;
            for ( int i = 1; i < 5; ++i )
                widths[i] = w5;
            redraw();
        }
    
    int mousePush( int X, int Y )
    {
        if ( value() < 0 ) return 0;

        Fl_Menu_Button menu(X,Y,0,0);

        menu.add( _("Copy/Color"),
                  FL_COMMAND + 'C',
                  (Fl_Callback*)copy_color_cb, (void*)this, 0);

        menu.popup();
        return 1;
    }

    bool valid_value()
    {
        int line = value();
        if (( line < 2 || line > 11 ) ||
            ( line > 5 && line < 8 ))
        {
            value(-1);
            return false;
        }
        _value = line;
        return true;
    }

    void draw()
    {
        value( _value );
        mrv::Browser::draw();
    }

    int handle( int event )
    {
        int ok = 0;
        switch( event )
        {
        case FL_PUSH:
            {
            if ( Fl::event_button() == 3 )
                return mousePush( Fl::event_x(), Fl::event_y() );
            ok = Fl_Browser::handle( event );
            if ( valid_value() ) return 1;
            return ok;
            }
        case FL_ENTER:
            return 1;
        case FL_FOCUS:
            return 1;
        default:
            ok = Fl_Browser::handle( event );
            if ( valid_value() ) return 1;
            return ok;
        }
    }
};


class ColorWidget : public Fl_Box
{
    Fl_Browser* color_browser_;

public:
    ColorWidget( int X, int Y, int W, int H, const char* L = 0 ) :
    Fl_Box( X, Y, W, H, L )
    {
        box( FL_FRAME_BOX );
    }

    int mousePush( int X, int Y )
    {
        color_browser_->value( 4 );

        Fl_Menu_Button menu( X, Y, 0, 0 );

        menu.add( _("Copy/Color"),
                  FL_COMMAND + 'C',
                  (Fl_Callback*)copy_color_cb, (void*)color_browser_, 0);

        menu.popup();
        return 1;
    }

    void color_browser( Fl_Browser* b ) {
        color_browser_ = b;
    }

    int handle( int event )
    {
        switch( event )
        {
        case FL_PUSH:
            if ( Fl::event_button() == FL_RIGHT_MOUSE )
                return mousePush( Fl::event_x(), Fl::event_y() );
        default:
            return Fl_Box::handle( event );
        }
    }
};


ColorInfo::ColorInfo( int X, int Y, int W, int h, const char* l ) :
    Fl_Group( X, Y, W, h, l )
{
    tooltip( _("Mark an area in the image with SHIFT + the left mouse button") );
    area = new Fl_Box( X, Y, W, 50 );
    area->box( FL_FLAT_BOX );
    area->align( FL_ALIGN_CENTER | FL_ALIGN_INSIDE );

    dcol = new ColorWidget( X+16, Y+10, 32, 32 );


    int WL, WH;
    fl_font( FL_HELVETICA, 14 );
    fl_measure( _("Maximum:"), WL, WH );
    WL += 8;
    int width = W - WL;
    int w5 = width / 4;
    int* col_widths = new int[6];
    col_widths[0] = WL;
    for ( int i = 1; i < 5; ++i )
        col_widths[i] = w5;
    col_widths[5] = 0;
    browser = new ColorBrowser( X, Y+10+area->h(), W, h - area->h() );
    browser->column_widths( col_widths );
    browser->showcolsep( 1 );
    browser->type(FL_HOLD_BROWSER);
    browser->resizable(browser);
    resizable(this);

    dcol->color_browser( browser );

}

void ColorInfo::main( ViewerUI* m ) {
    ui = m;
    browser->main(m);
}

int  ColorInfo::handle( int event )
{
    return Fl_Group::handle( event );
}


void ColorInfo::update(  const area::Info& info )
{
    DBG;
    if ( !visible_r() || info.box.min == info.box.max ) return;

    DBGM1( "ColorInfo= " << this );

    DBG;

    area->label( "" );

    unsigned numPixels = info.box.w() * info.box.h();
        
    std::ostringstream text;
    text << std::endl
         << _("Area") << ": (" << info.box.min.x
         << ", " << info.box.min.y
         << ") - (" << info.box.max.x
         << ", " << info.box.max.y << ")" << std::endl
         << _("Size") << ": [ " << info.box.w() << "x" << info.box.h()
         << " ] = "
         << numPixels << " "
         << ( numPixels == 1 ? _("pixel") : _("pixels") )
         << std::endl;
    area->copy_label( text.str().c_str() );

    mrv::BrightnessType brightness_type = (mrv::BrightnessType)
                                          ui->uiLType->value();



    static const char* kR = "@C4286611456@c";
    static const char* kG = "@C1623228416@c";
    static const char* kB = "@C2155937536@c";
    static const char* kA = "@C2964369408@c";

    static const char* kH = "@C2964324352@c";
    static const char* kS = "@C2964324352@c";
    static const char* kV = "@C2964324352@c";
    static const char* kL = "@C2964324352@c";




    DBG;
    Fl_Color col;
    {
        float r = info.rgba.mean.r;
        float g = info.rgba.mean.g;
        float b = info.rgba.mean.b;

        if ( r > 1.f ) r = 1.0f;
        if ( g > 1.f ) g = 1.0f;
        if ( b > 1.f ) b = 1.0f;
            
        if ( r <= 0.01f && g <= 0.01f && b <= 0.01f )
            col = FL_BLACK;
        else
        {
            col = fl_rgb_color((uchar)(r*255),
                               (uchar)(g*255),
                               (uchar)(b*255));
        }
    }
            
    dcol->color( col );
    dcol->redraw();
    DBG;

    char buf[24];
    text.str("");
    text.str().reserve(1024);
    text << "@b\t"
         << kR
         << N_("R") << "\t"
         << kG
         << N_("G") << "\t"
         << kB
         << N_("B") << "\t"
         << kA
         << N_("A")
         << std::endl
         << _("Maximum") << ":\t@c"
         << float_printf( buf, info.rgba.max.r ) << "\t@c"
         << float_printf( buf, info.rgba.max.g ) << "\t@c"
         << float_printf( buf, info.rgba.max.b ) << "\t@c"
         << float_printf( buf, info.rgba.max.a ) << std::endl
         << _("Minimum") << ":\t@c"
         << float_printf( buf, info.rgba.min.r ) << "\t@c"
         << float_printf( buf, info.rgba.min.g ) << "\t@c"
         << float_printf( buf, info.rgba.min.b ) << "\t@c"
         << float_printf( buf, info.rgba.min.a ) << std::endl;

    text << _("Range") << ":\t@c"
         << float_printf( buf, info.rgba.diff.r ) << "\t@c"
         << float_printf( buf, info.rgba.diff.g ) << "\t@c"
         << float_printf( buf, info.rgba.diff.b ) << "\t@c"
         << float_printf( buf, info.rgba.diff.a ) << std::endl
         << "@b" << _("Mean") << ":\t@c"
         << kR
         << float_printf( buf, info.rgba.mean.r ) << "\t@c"
         << kG
         << float_printf( buf, info.rgba.mean.g ) << "\t@c"
         << kB
         << float_printf( buf, info.rgba.mean.b ) << "\t@c"
         << kA
         << float_printf( buf, info.rgba.mean.a ) << std::endl
         << std::endl
         << "@b\t";

    DBG;

    switch( ui->uiBColorType->value()+1 )
    {
    case color::kITU_709:
        text << kH << N_("7") << "\t@c"
             << kS << N_("0") << "\t@c"
             << kL << N_("9");
        break;
    case color::kITU_601:
        text << kH << N_("6") << "\t@c"
             << kS << N_("0") << "\t@c"
             << kL << N_("1");
        break;
    case color::kYIQ:
        text << kH << N_("Y") << "\t@c"
             << kS << N_("I") << "\t@c"
             << kL << N_("Q");
        break;
    case color::kYDbDr:
        text << kH << N_("Y") << "\t@c"
             << kS << N_("Db") << "\t@c"
             << kL << N_("Dr");
        break;
    case color::kYUV:
        text << kH << N_("Y") << "\t@c"
             << kS << N_("U") << "\t@c"
             << kL << N_("V");
        break;
    case color::kCIE_Luv:
        text << kH << N_("L") << "\t@c"
             << kS << N_("u") << "\t@c"
             << kL << N_("v");
        break;
    case color::kCIE_Lab:
        text << kH << N_("L") << "\t@c"
             << kS << N_("a") << "\t@c"
             << kL << N_("b");
        break;
    case color::kCIE_xyY:
        text << kH << N_("x") << "\t@c"
             << kS << N_("y") << "\t@c"
             << kL << N_("Y");
        break;
    case color::kCIE_XYZ:
        text << kH << N_("X") << "\t@c"
             << kS << N_("Y") << "\t@c"
             << kL << N_("Z");
        break;
    case color::kHSL:
        text << kH << N_("H") << "\t@c"
             << kS << N_("S") << "\t@c"
             << kL << N_("L");
        break;
    case color::kHSV:
    default:
        text << kH << N_("H") << "\t@c"
             << kS << N_("S") << "\t@c"
             << kV << N_("V");
        break;
    }

    text << "\t" << kL;
    DBG;

    switch( brightness_type )
    {
    case kAsLuminance:
        text << N_("Y");
        break;
    case kAsLumma:
        text << N_("Y'");
        break;
    case kAsLightness:
        text << N_("L");
        break;
    }


    DBG;
    text << std::endl
         << _("Maximum") << ":\t@c"
         << float_printf( buf, info.hsv.max.r ) << "\t@c"
         << float_printf( buf, info.hsv.max.g ) << "\t@c"
         << float_printf( buf, info.hsv.max.b ) << "\t@c"
         << float_printf( buf, info.hsv.max.a ) << std::endl
         << _("Minimum") << ":\t@c"
         << float_printf( buf, info.hsv.min.r ) << "\t@c"
         << float_printf( buf, info.hsv.min.g ) << "\t@c"
         << float_printf( buf, info.hsv.min.b ) << "\t@c"
         << float_printf( buf, info.hsv.min.a ) << std::endl;

    text << _("Range") << ":\t@c"
         << float_printf( buf, info.hsv.diff.r ) << "\t@c"
         << float_printf( buf, info.hsv.diff.g ) << "\t@c"
         << float_printf( buf, info.hsv.diff.b ) << "\t@c"
         << float_printf( buf, info.hsv.diff.a ) << std::endl
         << "@b" << _("Mean") << ":\t@c"
         << kH
         << float_printf( buf, info.hsv.mean.r ) << "\t@c"
         << kS
         << float_printf( buf, info.hsv.mean.g ) << "\t@c"
         << kV
         << float_printf( buf, info.hsv.mean.b ) << "\t@c"
         << kL
         << float_printf( buf, info.hsv.mean.a );

    DBG;
    stringArray lines;
    mrv::split_string( lines, text.str(), "\n" );
    stringArray::iterator i = lines.begin();
    stringArray::iterator e = lines.end();
    area->redraw_label();

    
    browser->clear();
    for ( ; i != e; ++i )
    {
        browser->add( (*i).c_str() );
    }
    DBG;

    browser->redraw();
    DBG;
}

}  // namespace mrv