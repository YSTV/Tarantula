/******************************************************************************
*   Copyright (C) 2011 - 2013  York Student Television
*
*   Tarantula is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   Tarantula is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with Tarantula.  If not, see <http://www.gnu.org/licenses/>.
*
*   Contact     : tarantula@ystv.co.uk
*
*   File Name   : EventProcessor_Demo.cpp
*   Version     : 1.0
*   Description : Demonstration EventProcessor. Generates events to run on
*                 VideoDevice_Demo
*
*****************************************************************************/


#include "EventProcessor_Demo.h"
#include "PluginConfig.h"

/**
 * A sample EventProcessor, should spit back the original event on the first device,
 * plus a child on the same device.
 *
 * @param config Configuration details for this plugin
 * @param h Link back to GlobalStuff structures
 */
EventProcessor_Demo::EventProcessor_Demo (PluginConfig config, Hook h) :
        MouseCatcherProcessorPlugin(config, h)
{
    // Register the preprocessor
    g_preprocessorlist.emplace("EventProcessor_Demo::demoPreProcessor", &EventProcessor_Demo::demoPreProcessor);

    // In this case no data is needed as this is a demo.
    m_status = READY;
}

EventProcessor_Demo::~EventProcessor_Demo ()
{
    g_preprocessorlist.erase("EventProcessor_Demo::demoPreProcessor");
}

/**
 * Handle an incoming event directed at this plugin
 * @param originalEvent MouseCatcherEvent The event which spawned this plugin call
 * @param resultingEvents vector<MouseCatcherEvent>* A pointer to a vector of events generated by this call
 */
void EventProcessor_Demo::handleEvent (MouseCatcherEvent originalEvent,
        MouseCatcherEvent& resultingEvent)
{
    if (READY != m_status)
    {
        m_hook.gs->L->error(m_pluginname, "Plugin not in ready state");
        return;
    }

    resultingEvent = originalEvent;

    // Create a child
    MouseCatcherEvent childevent;
    childevent.m_channel = "Default";
    childevent.m_duration = 1;
    childevent.m_action = 0;
    childevent.m_extradata["output"] = "Default";
    childevent.m_extradata["input"] = "Inform";
    childevent.m_targetdevice = "Demo Crosspoint 1";
    childevent.m_triggertime = originalEvent.m_triggertime;
    childevent.m_preprocessor = "EventProcessor_Demo::demoPreProcessor";
    childevent.m_description = "Demonstration child event from EP";
    resultingEvent.m_childevents.push_back(childevent);

}

/**
 * Demo function to test PreProcessor functionality
 *
 * @param event    Reference to playlist event that called the processor
 * @param pchannel Pointer to channel calling the function
 */
void EventProcessor_Demo::demoPreProcessor (PlaylistEntry &event, Channel *pchannel)
{
    g_logger.info("Demo PreProcessor", "Demo PreProcessor is running");
}

extern "C"
{
    void LoadPlugin (Hook h, PluginConfig config, std::shared_ptr<Plugin>& pluginref)
    {
        //must declare as pointer to avoid object being deleted once function call is complete!
        std::shared_ptr<EventProcessor_Demo> plugtemp = std::make_shared<EventProcessor_Demo>(config, h);
        pluginref = std::dynamic_pointer_cast<Plugin>(plugtemp);
    }
}
