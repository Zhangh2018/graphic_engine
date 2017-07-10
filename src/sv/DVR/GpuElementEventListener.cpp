/*------------------------------------------------------------------------------------------
 * FILE: GpuElementEventListener.cpp
 *==========================================================================================
 * Copyright 2017  O-Film Technologies, Inc., All Rights Reserved.
 * O-Film Confidential
 *
 * DESCRIPTION:
 *
 * ABBREVIATIONS:
 *   TODO: List of abbreviations used, or reference(s) to external document(s)
 *
 * TRACEABILITY INFO:
 *   Design Document(s):
 *     TODO: Update list of design document(s)
 *
 *   Requirements Document(s):
 *     TODO: Update list of requirements document(s)
 *
 *   Applicable Standards (in order of precedence: highest first):
 *
 * DEVIATIONS FROM STANDARDS:
 *   TODO: List of deviations from standards in this file, or
 *   None.
 * VERSION: 10 7月 2017 dota2_black 
 *------------------------------------------------------------------------------------------*/
#include "GpuElementEventListener.h"

namespace GUI
{
    CGPUEventListener::CGPUEventListener()
        :IGUIElement("CGPUEventListener")
        ,CXrBaseView()
        ,m_eventLayer(NULL)
    {
    }
    CGPUEventListener::~CGPUEventListener()
    {
    }
    bool CGPUEventListener::Create(const uint32_t pos_x, const uint32_t pos_y,
                          const uint32_t element_width, const uint32_t element_height)
    {
        const IUINode* node = GetLayoutNode();
        if(node)
        {
            /** 第一步: 创建进度条的base Layer*/
            InsertFlag flag = InsertFlag_Child;
            int32_t parent = GetParent();
            if(parent < 0)
            {
                //控件不存在父节点id
                parent = -1;
                flag = InsertFlag_Default;
            }
            Int32 m_eventLayerId = node->CreateSpirit(parent, flag, -1, 0, pos_x, pos_y, 0, element_width, element_height);
            m_eventLayer = node->GetLayer(m_eventLayerId);
            m_eventLayer->SetEventResponder(this);
            
            IGUIElement::SetHwnd((GUI_HANDLE_T)m_eventLayerId);
        }
    }

    Boolean CGPUEventListener::OnTouchEvent(Int32 layerId, Int32 x, Int32 y, Int32 type)
    {
        if(type == TouchEvent_Down)
        {
            IGUIElement::DispatchEvent(IGUIElement::EventId(), type);
        }
        return TRUE;
    }

    void CGPUEventListener::Enable(bool enable)
    {
        m_eventLayer->SetEnable(enable);
    }
    IMPLEMENT_DYNAMIC_CLASS(CGPUEventListener)
};
/*------------------------------------------------------------------------------------------
 * File Revision History (top to bottom: first revision to last revision)
 *==========================================================================================
 *
 * Date             SCR                      user                              Description
 *==========================================================================================

 *------------------------------------------------------------------------------------------*/