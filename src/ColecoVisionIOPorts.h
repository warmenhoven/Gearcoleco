/*
 * Gearcoleco - ColecoVision Emulator
 * Copyright (C) 2021  Ignacio Sanchez

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/
 *
 */

#ifndef COLECOVISIONIOPORTS_H
#define	COLECOVISIONIOPORTS_H

#include "IOPorts.h"

class Audio;
class Video;
class Input;
class Cartridge;
class Memory;

class ColecoVisionIOPorts : public IOPorts
{
public:
    ColecoVisionIOPorts(Audio* pAudio, Video* pVideo, Input* pInput, Cartridge* pCartridge, Memory* pMemory);
    ~ColecoVisionIOPorts();
    void Reset();
    u8 In(u8 port);
    void Out(u8 port, u8 value);
    void SaveState(std::ostream& stream);
    void LoadState(std::istream& stream);
private:
    Audio* m_pAudio;
    Video* m_pVideo;
    Input* m_pInput;
    Cartridge* m_pCartridge;
    Memory* m_pMemory;
};

#include "Video.h"
#include "Audio.h"
#include "Input.h"
#include "Cartridge.h"
#include "Memory.h"

inline u8 ColecoVisionIOPorts::In(u8 port)
{
    switch(port & 0xE0) {
        case 0xA0:
        {
            if (port & 0x01)
            {
                return m_pVideo->GetStatusFlags();
            }
            else
            {
                return m_pVideo->GetDataPort();
            }
            break;
        }
        case 0xE0:
        {
            return 0xFF;
        }
    }

    Log("--> ** Attempting to read from port $%X", port);

    return 0xFF;
}

inline void ColecoVisionIOPorts::Out(u8 port, u8 value)
{
    switch(port & 0xE0) {
        case 0x80:
        {
            break;
        }
        case 0xA0:
        {
            if (port & 0x01)
            {
                m_pVideo->WriteControl(value);
            }
            else
            {
                m_pVideo->WriteData(value);
            }
            break;
        }
        case 0xC0:
        {
            break;
        }
        case 0xE0:
        {
            m_pAudio->WriteAudioRegister(value);
            break;
        }
        default:
        {
            Log("--> ** Output to port $%X: %X", port, value);
        }
    }
}

#endif	/* COLECOVISIONIOPORTS_H */
