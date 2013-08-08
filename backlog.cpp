/*
 * Copyright (C) 2004-2013 ZNC, see the NOTICE file for details.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <znc/FileUtils.h>
#include <znc/Client.h>
#include <znc/Chan.h>
#include <znc/Modules.h>

using std::vector;


class CBacklogMod : public CModule {
public:
	MODCONSTRUCTOR(CBacklogMod) {}

	virtual bool OnLoad(const CString& sArgs, CString& sMessage) {
		PutModule("I'm being loaded with the arguments: [" + sArgs + "]");
		//AddTimer(new CSampleTimer(this, 300, 0, "Sample", "Sample timer for sample things."));
		//AddTimer(new CSampleTimer(this, 5, 20, "Another", "Another sample timer."));
		//AddTimer(new CSampleTimer(this, 25000, 5, "Third", "A third sample timer."));
		return true;
	}

	virtual ~CBacklogMod() {
		PutModule("I'm being unloaded!");
	}

	virtual bool OnBoot() {
		// This is called when the app starts up (only modules that are loaded in the config will get this event)
		return true;
	}


	virtual void OnModCommand(const CString& sCommand) {
		/*
		if (sCommand.Equals("TIMERS")) {
			ListTimers();
		}
		*/
		// TODO: sanity check on file path, see log.cpp

		CFile LogFile(sCommand);
		CString Line;

		if (LogFile.Open()) {
			while (LogFile.ReadLine(Line)) {
				PutModule(Line);
			}
		} else {
			PutModule("Could not open log file"); //  [" << sCommand << "]: " << strerror(errno));
		}

		LogFile.Close();
	}
};

template<> void TModInfo<CBacklogMod>(CModInfo& Info) {
	Info.SetWikiPage("sample");
	Info.SetHasArgs(true);
	Info.SetArgsHelpText("Takes %Channelname% and %number of lines% as arguments");
}

NETWORKMODULEDEFS(CBacklogMod, "Module for getting the last X lines of a channels log.")

