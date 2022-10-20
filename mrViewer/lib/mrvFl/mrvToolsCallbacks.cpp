
#include <FL/Fl_Check_Button.H>

#include "mrvFl/mrvDockGroup.h"
#include "mrvFl/mrvToolsCallbacks.h"
#include "mrvFl/mrvToolGroup.h"
#include "mrvFl/mrvHorSlider.h"
#include "mrvFl/mrvCollapsibleGroup.h"

#include "mrViewer.h"

namespace mrv
{
    void color_tool_grp( Fl_Widget* w, ViewerUI* ui )
    {
        DockGroup* dock = ui->uiDock;
        ResizableBar* bar = ui->uiResizableBar;
        Fl_Group* dg = ui->uiDockGroup;
        ToolGroup *g = new ToolGroup(dock, 0, dock->x(), dock->y(),
                                     dg->w()-bar->w(), 420, "Color");
        g->begin();

        CollapsibleGroup* cg = new CollapsibleGroup( g->x(), 20, g->w(), 20,
                                                     "Color Controls" );
        Fl_Button* b = cg->button();
        b->labelsize(14);
        b->size(b->w(), 18);
        cg->begin();
        
        Fl_Check_Button* c;
        HorSlider* s;
        auto cV = new Widget< Fl_Check_Button >( g->x()+90, 50,
                                                            g->w(), 20,
                                                            "Enabled" );
        c = cV;
        c->labelsize(12);
        cV->callback( [=]( auto w ) {
            timeline::DisplayOptions& o = ui->uiView->getDisplayOptions(0);
            o.colorEnabled = w->value();
            ui->uiView->redraw();
        } );
        
        
        auto sV = new Widget< HorSlider >( g->x(), 90, g->w(), 20, "Add" );
        s = sV;
        s->setRange( 0.f, 4.0f );
        s->setDefaultValue( 0.0f );
        sV->callback( [=]( auto w ) {
            timeline::DisplayOptions& o = ui->uiView->getDisplayOptions(0);
            float f = w->value();
            o.color.add = math::Vector3f( f, f, f );
            ui->uiView->redraw();
        } );
        
        sV = new Widget< HorSlider >( g->x(), 90, g->w(), 20, "Brightness" );
        s = sV;
        s->setRange( 0.f, 4.0f );
        s->setDefaultValue( 1.0f );
        sV->callback( [=]( auto w ) {
            timeline::DisplayOptions& o = ui->uiView->getDisplayOptions(0);
            float g = ui->uiGain->value();
            float f = w->value() * g;
            o.color.brightness = math::Vector3f( f, f, f );
            ui->uiView->redraw();
        } );
        
        
        sV = new Widget< HorSlider >( g->x(), 90, g->w(), 20, "Contrast" );
        s = sV;
        s->setRange( 0.f, 4.0f );
        s->setDefaultValue( 1.0f );
        sV->callback( [=]( auto w ) {
            timeline::DisplayOptions& o = ui->uiView->getDisplayOptions(0);
            float f = w->value();
            o.color.contrast = math::Vector3f( f, f, f );
            ui->uiView->redraw();
        } );
        
        sV = new Widget< HorSlider >( g->x(), 90, g->w(), 20, "Saturaion" );
        s = sV;
        s->setRange( 0.f, 4.0f );
        s->setDefaultValue( 1.0f );
        sV->callback( [=]( auto w ) {
            timeline::DisplayOptions& o = ui->uiView->getDisplayOptions(0);
            float f = w->value();
            o.color.saturation = math::Vector3f( f, f, f );
            ui->uiView->redraw();
        } );
        
        sV = new Widget< HorSlider >( g->x(), 90, g->w(), 20, "Tint" );
        s = sV;
        s->setRange( 0.f, 1.0f );
        s->setStep( 0.01 );
        s->setDefaultValue( 0.0f );
        sV->callback( [=]( auto w ) {
            timeline::DisplayOptions& o = ui->uiView->getDisplayOptions(0);
            o.color.tint = w->value();
            ui->uiView->redraw();
        } );
        
        cV = new Widget< Fl_Check_Button >( g->x()+90, 50, g->w(), 20,
                                            "Invert" );
        c = cV;
        c->labelsize(12);
        cV->callback( [=]( auto w ) {
            timeline::DisplayOptions& o = ui->uiView->getDisplayOptions(0);
            o.color.invert = w->value();
            ui->uiView->redraw();
        } );

        cg->end();

        cg = new CollapsibleGroup( g->x(), 180, g->w(), 20, "Levels" );
        b = cg->button();
        b->labelsize(14);
        b->size(b->w(), 18);
        cg->layout();

        cg->begin();
        
        cV = new Widget< Fl_Check_Button >( g->x()+90, 50, g->w(), 20,
                                            "Enabled" );
        c = cV;
        c->labelsize(12);
        cV->callback( [=]( auto w ) {
            timeline::DisplayOptions& o = ui->uiView->getDisplayOptions(0);
            o.levelsEnabled = w->value();
            ui->uiView->redraw();
        } );

        sV = new Widget< HorSlider >( g->x(), 90, g->w(), 20, "In Low" );
        s = sV;
        s->setRange( 0.f, 1.0f );
        s->setStep( 0.01 );
        s->setDefaultValue( 0.0f );
        sV->callback( [=]( auto w ) {
            timeline::DisplayOptions& o = ui->uiView->getDisplayOptions(0);
            o.levels.inLow = w->value();
            ui->uiView->redraw();
        } );
        
        
        sV = new Widget< HorSlider >( g->x(), 90, g->w(), 20, "In High" );
        s = sV;
        s->setRange( 0.f, 1.0f );
        s->setStep( 0.01 );
        s->setDefaultValue( 1.0f );
        sV->callback( [=]( auto w ) {
            timeline::DisplayOptions& o = ui->uiView->getDisplayOptions(0);
            o.levels.inHigh = w->value();
            ui->uiView->redraw();
        } );
        
        sV = new Widget< HorSlider >( g->x(), 90, g->w(), 20, "Gamma" );
        s = sV;
        s->setRange( 0.f, 6.0f );
        s->setStep( 0.01 );
        s->setDefaultValue( 1.0f );
        s->value( ui->uiGain->value() );
        
        sV->callback( [=]( auto w ) {
            timeline::DisplayOptions& o = ui->uiView->getDisplayOptions(0);
            float f = w->value();
            o.levels.gamma = f;
            ui->uiGamma->value( f );
            ui->uiGammaInput->value( f );
            ui->uiView->redraw();
        } );
        
        sV = new Widget< HorSlider >( g->x(), 90, g->w(), 20, "Out Low" );
        s = sV;
        s->setRange( 0.f, 1.0f );
        s->setStep( 0.01 );
        s->setDefaultValue( 0.0f );
        sV->callback( [=]( auto w ) {
            timeline::DisplayOptions& o = ui->uiView->getDisplayOptions(0);
            o.levels.outLow = w->value();
            ui->uiView->redraw();
        } );
        
        
        sV = new Widget< HorSlider >( g->x(), 90, g->w(), 20, "Out High" );
        s = sV;
        s->setRange( 0.f, 1.0f );
        s->setStep( 0.01 );
        s->setDefaultValue( 1.0f );
        sV->callback( [=]( auto w ) {
            timeline::DisplayOptions& o = ui->uiView->getDisplayOptions(0);
            o.levels.inHigh = w->value();
            ui->uiView->redraw();
        } );
        
        cg->end();
        cg = new CollapsibleGroup( g->x(), 180, g->w(), 20, "Soft Clip" );
        b = cg->button();
        b->labelsize(14);
        b->size(b->w(), 18);
        cg->layout();

        cg->begin();

        c = new Fl_Check_Button( g->x()+90, 160, g->w(), 20, "Enabled" );
        c->labelsize(12);
        
        s = new mrv::HorSlider( g->x(), 140, g->w(), 20, "Soft Clip" );
        s->setDefaultValue( 0.0f );
        
        
        
        cg->end();
        
        g->end();
        g->box( FL_FLAT_BOX );
        g->redraw();
    }
}
