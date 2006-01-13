/***************************************************************************
 *   Copyright (C) 2005 by Joris Guisson                                   *
 *   joris.guisson@gmail.com                                               *
 *   ivasic@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Steet, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/
#ifndef IPBLOCKINGPREFPAGE_H
#define IPBLOCKINGPREFPAGE_H

#include <interfaces/prefpageinterface.h>
#include "ipblockingpref.h"
#include "ipfilterplugin.h"
#include <interfaces/coreinterface.h>
#include <qthread.h>

class KProgress;

namespace kt
{
	class IPFilterPlugin;
	
	class ConvertThread : public QThread
	{
		public:
			
			ConvertThread(KProgress* kp, QLabel* lbl, IPFilterPlugin* p);
			virtual void run();
			
		private:
			KProgress* progress;
			QLabel* lblProgress;
			IPFilterPlugin* m_plugin;
	};
	
	
	/**
	@author Ivan Vasic
	*/
	class IPBlockingPrefPageWidget : public IPBlockingPref
	{
		public:
			IPBlockingPrefPageWidget(QWidget *parent = 0);
			void apply();
			void convert();
			void setPlugin(IPFilterPlugin* p);
			
		public slots:
    		virtual void btnDownload_clicked();
    		virtual void checkUseKTfilter_toggled(bool);
    		virtual void checkUseLevel1_toggled(bool);
			
		private:
			IPFilterPlugin* m_plugin;
	};
	
	/**
	 * @author Ivan Vasic
	 * @brief IPBlocking plugin interface page
	 **/
	class IPBlockingPrefPage : public PrefPageInterface
	{
		public:
			IPBlockingPrefPage(CoreInterface* core, IPFilterPlugin* p);
			virtual ~IPBlockingPrefPage();
			
			virtual bool apply();
			virtual void createWidget(QWidget* parent);
			virtual void updateData();
			virtual void deleteWidget();
			
			void loadFilters();
			void unloadFilters();

		private:
			CoreInterface* m_core;
			IPBlockingPrefPageWidget* widget;
			IPFilterPlugin* m_plugin;
	};
}
#endif
