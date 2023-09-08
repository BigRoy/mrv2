// SPDX-License-Identifier: BSD-3-Clause
// mrv2
// Copyright Contributors to the mrv2 Project. All rights reserved.

#include <cstring> // for strcpy

#include "mrvCore/mrvHome.h"

#include "mrvPanels/mrvPanelsCallbacks.h"

#include "mrvWidgets/mrvLogDisplay.h"

#include "mrvFl/mrvIO.h"

#include "mrvFLU/Flu_File_Chooser.h"

#include "mrvCore/mrvOS.h"

#include "mrvApp/App.h"

namespace mrv
{

    namespace trace
    {

        std::fstream logbuffer::out;
        bool logbuffer::_debug = true;

        logbuffer::~logbuffer()
        {
            if (out.is_open())
                out.close();
        };

        void logbuffer::open_log_panel()
        {
            if (!App::ui)
                return;

            if (Flu_File_Chooser::window)
                return;

            if (LogDisplay::prefs == LogDisplay::kDockOnError)
            {
                if (!logsPanel)
                    logs_panel_cb(NULL, App::ui);
                if (!logsPanel->is_panel())
                    logsPanel->dock();
            }
            else if (LogDisplay::prefs == LogDisplay::kWindowOnError)
            {
                if (!logsPanel)
                    logs_panel_cb(NULL, App::ui);
                if (logsPanel->is_panel())
                    logsPanel->undock();
            }
        }

        int logbuffer::sync()
        {
            if (!pbase())
                return 0;

            // make sure to null terminate the string
            sputc('\0');

            // freeze and call the virtual print method
            char* c = strdup(str().c_str());
            if (!c)
                return 1;

            if (_debug && out.is_open())
                out << c << std::flush;

            print(c);

            free(c);

            // reset iterator to first position & unfreeze
            seekoff(0, std::ios::beg);
            return 0;
        }

        void logbuffer::open_stream()
        {
            if (!out.is_open())
            {
                std::string file = mrv::homepath();
                file += "/.filmaura/errorlog.txt";
                out.open(file.c_str(), std::ios_base::out);
            }
            if (out.is_open())
            {
                out << "DEBUG LOG" << std::endl
                    << "=========" << std::endl
                    << std::endl;
            }
        }

        void errorbuffer::print(const char* c)
        {
            std::cerr << c;
            open_log_panel();
            if (uiLogDisplay)
                uiLogDisplay->error(c);
        }

        void warnbuffer::print(const char* c)
        {
            std::cerr << c;
            if (uiLogDisplay)
                uiLogDisplay->warning(c);
        }

        void infobuffer::print(const char* c)
        {
            std::cout << c << std::flush;
            if (uiLogDisplay)
                uiLogDisplay->info(c);
        }

        infostream info;
        warnstream warn;
        errorstream error;
    } // namespace trace

} // namespace mrv
