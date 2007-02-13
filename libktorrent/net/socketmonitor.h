/***************************************************************************
 *   Copyright (C) 2005 by Joris Guisson                                   *
 *   joris.guisson@gmail.com                                               *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/
#ifndef NETSOCKETMONITOR_H
#define NETSOCKETMONITOR_H


#include <qmutex.h>
#include <qptrlist.h>
#include <util/constants.h>


namespace net
{
	using bt::Uint32;
	
	class BufferedSocket;
	class UploadThread;
	class DownloadThread;

	/**
	 * @author Joris Guisson <joris.guisson@gmail.com>
	 * 
	 * Monitors all sockets for upload and download traffic.
	 * It uses two threads to do this.
	*/
	class SocketMonitor 
	{
		static SocketMonitor self;

		QMutex mutex;
		UploadThread* ut;
		DownloadThread* dt;
		QPtrList<BufferedSocket> smap;
				
		SocketMonitor();	
	public:
		virtual ~SocketMonitor();
		
		/// Add a new socket, will start the threads if necessary
		void add(BufferedSocket* sock);
		
		/// Remove a socket, will stop threads if no more sockets are left
		void remove(BufferedSocket* sock);
		
		typedef QPtrList<BufferedSocket>::iterator Itr;
		
		/// Get the begin of the list of sockets
		Itr begin() {return smap.begin();}
		
		/// Get the end of the list of sockets
		Itr end() {return smap.end();}
		
		/// lock the monitor
		void lock();
		
		/// unlock the monitor
		void unlock();
		
		/// Tell upload thread a packet is ready
		void signalPacketReady();
		
		static void setDownloadCap(Uint32 bytes_per_sec);
		static void setUploadCap(Uint32 bytes_per_sec);
		static SocketMonitor & instance() {return self;}
	};

}

#endif
