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
 *   51 Franklin Steet, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/
#include <qstring.h>
#include <string.h>
#include "packet.h"
#include "request.h"
#include "chunk.h"
#include <util/bitset.h>
#include <util/functions.h>

namespace bt
{



	Packet::Packet(Uint8 type) : hdr_length(0),data(0),data_length(0),written(0),chunk(0)
	{
		WriteUint32(hdr,0,1);
		hdr[4] = type;
		hdr_length = 5;
	}
	
	Packet::Packet(Uint32 chunk) : hdr_length(0),data(0),data_length(0),written(0),chunk(0)
	{
		WriteUint32(hdr,0,5);
		hdr[4] = HAVE;
		WriteUint32(hdr,5,chunk);
		hdr_length = 9;
	}
	
	Packet::Packet(const BitSet & bs) : hdr_length(0),data(0),data_length(0),written(0),chunk(0)
	{
		WriteUint32(hdr,0,1 + bs.getNumBytes());
		hdr[4] = BITFIELD;
		hdr_length = 5;
		data_length = bs.getNumBytes();
		data = new Uint8[data_length];
		memcpy(data,bs.getData(),data_length);
	}
	
	Packet::Packet(const Request & r,bool cancel)
	: hdr_length(0),data(0),data_length(0),written(0),chunk(0)
	{
		WriteUint32(hdr,0,13);
		hdr[4] = cancel ? CANCEL : REQUEST;
		WriteUint32(hdr,5,r.getIndex());
		WriteUint32(hdr,9,r.getOffset());
		WriteUint32(hdr,13,r.getLength());
		hdr_length = 17;
	}
	
	Packet::Packet(Uint32 index,Uint32 begin,Uint32 len,Chunk* ch)
	: hdr_length(0),data(0),data_length(0),written(0),chunk(ch)
	{
		WriteUint32(hdr,0,9 + len);
		hdr_length = 13;
		data_length = len;
		
		hdr[4] = PIECE;
		WriteUint32(hdr,5,index);
		WriteUint32(hdr,9,begin);
		data = ch->getData() + begin;
		ch->ref();
	}


	Packet::~Packet()
	{
		if (chunk)
			chunk->unref();
		else
			delete [] data;
	}
	

	QString Packet::debugString() const
	{
		switch (hdr[4])
		{
			case CHOKE : return QString("CHOKE %1 %2").arg(hdr_length).arg(data_length);
			case UNCHOKE : return QString("UNCHOKE %1 %2").arg(hdr_length).arg(data_length);
			case INTERESTED : return QString("INTERESTED %1 %2").arg(hdr_length).arg(data_length);
			case NOT_INTERESTED : return QString("NOT_INTERESTED %1 %2").arg(hdr_length).arg(data_length);
			case HAVE : return QString("HAVE %1 %2").arg(hdr_length).arg(data_length);
			case BITFIELD : return QString("BITFIELD %1 %2").arg(hdr_length).arg(data_length);
			case PIECE : return QString("PIECE %1 %2").arg(hdr_length).arg(data_length);
			case REQUEST : return QString("REQUEST %1 %2").arg(hdr_length).arg(data_length);
			case CANCEL : return QString("CANCEL %1 %2").arg(hdr_length).arg(data_length);
			default: return QString("UNKNOWN %1 %2").arg(hdr_length).arg(data_length);
		}
	}
	
	bool Packet::isOK() const
	{
		if (getDataLength() > 0 && !getData())
			return false;
		
		if (hdr[4] == PIECE && !chunk)
			return false;
		
		if (hdr[4] == PIECE && !chunk->getData())
			return false;
		
		return true;
	}

}
