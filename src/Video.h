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

#ifndef VIDEO_H
#define	VIDEO_H

#include "definitions.h"

class Memory;
class Processor;

class Video
{
public:
    Video(Memory* pMemory, Processor* pProcessor);
    ~Video();
    void Init();
    void Reset(bool bPAL);
    bool Tick(unsigned int clockCycles);
    u8 GetDataPort();
    u8 GetStatusFlags();
    void WriteData(u8 data);
    void WriteControl(u8 control);
    void SaveState(std::ostream& stream);
    void LoadState(std::istream& stream);
    u8* GetVRAM();
    u8* GetRegisters();
    u16* GetFrameBuffer();
    int GetMode();
    void Render24bit(u16* srcFrameBuffer, u8* dstFrameBuffer, GC_Color_Format pixelFormat, int size);
    void Render16bit(u16* srcFrameBuffer, u8* dstFrameBuffer, GC_Color_Format pixelFormat, int size);

private:
    void IncrementAddress();
    void ScanLine(int line);
    void RenderBackground(int line);
    void RenderSprites(int line);
    void InitPalettes();

private:
    Memory* m_pMemory;
    Processor* m_pProcessor;
    u8* m_pInfoBuffer;
    u16* m_pFrameBuffer;
    u8* m_pVdpVRAM;
    bool m_bFirstByteInSequence;
    u8 m_VdpRegister[8];
    u8 m_VdpBuffer;
    u16 m_VdpAddress;
    int m_iCycleCounter;
    u8 m_VdpStatus;
    int m_iLinesPerFrame;
    bool m_bPAL;
    int m_iMode;
    int m_iRenderLine;
    int m_iScreenWidth;

    struct LineEvents 
    {
        bool vint;
        bool vintFlag;
        bool render;
        bool display;
        bool spriteovr;
    };

    LineEvents m_LineEvents;

    enum Timing
    {
        TIMING_VINT = 0,
        TIMING_XSCROLL = 1,
        TIMING_HINT = 2,
        TIMING_VCOUNT = 3,
        TIMING_FLAG_VINT = 4,
        TIMING_RENDER = 5,
        TIMING_DISPLAY = 6,
        TIMING_SPRITEOVR = 7
    };

    int m_Timing[8];
    int m_NextLineSprites[8];
    bool m_bDisplayEnabled;
    bool m_bSpriteOvrRequest;

    u16 m_SG1000_palette_565_rgb[16];
    u16 m_SG1000_palette_555_rgb[16];
    u16 m_SG1000_palette_565_bgr[16];
    u16 m_SG1000_palette_555_bgr[16];
};

inline u8* Video::GetVRAM()
{
    return m_pVdpVRAM;
}

inline u8* Video::GetRegisters()
{
    return m_VdpRegister;
}

inline int Video::GetMode()
{
    return m_iMode;
}

inline u16* Video::GetFrameBuffer()
{
    return m_pFrameBuffer;
}

const u8 kPalette_888[48] = {0,0,0, 0,0,0, 33,200,66, 94,220,120, 84,85,237, 125,118,252, 212,82,77, 66,235,245, 252,85,84, 255,121,120, 212,193,84, 230,206,128, 33,176,59, 201,91,186, 204,204,204, 255,255,255};
const u8 k2bitTo8bit[4] = {0,85,170,255};
const u8 k2bitTo5bit[4] = {0,10,21,31};
const u8 k2bitTo6bit[4] = {0,21,42,63};
const u8 k4bitTo8bit[16] = {0,17,34,51,68,86,102,119,136,153,170,187,204,221,238,255};
const u8 k4bitTo5bit[16] = {0,2,4,6,8,10,12,14,17,19,21,23,25,27,29,31};
const u8 k4bitTo6bit[16] = {0,4,8,13,17,21,25,29,34,38,42,46,50,55,59,63};

#endif	/* VIDEO_H */
