/*------------------------------------------------------------------------------------------
 * FILE: DVR_Layout.cpp
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
 * VERSION: 31 5月 2017 dota2_black 
 *------------------------------------------------------------------------------------------*/
#include "DVR_Layout.h"
#include "IGUIElement.h"
#include "GPU_Module_Interface.h"

namespace GUI
{
    DVR_Layout* DVR_Layout::m_layout = NULL;
    AvmEventType   DVR_Layout::m_event_type = AvmEvent::Invalid_Event_Type;
    IDVR* DVR_Layout::GetLayout()
    {
        if(m_layout == NULL)
        {
            m_layout = new DVR_Layout;
        }
        return m_layout;
    }

    /**
     * \beief gpu控件元素信息表, 记录控件消息事件响应操作 
     */    
    struct ILayout::ElementFuntionTable DVR_Layout::m_element_info[] = 
    { 
        { "CGPUPanel"      , "多媒体栏"   , 1, 0, NULL, (PFCreateElement)(&DVR_Layout::InitMediaPanel)       , NULL, NULL},
        { "CGPUButton"     , "播放按钮"   , 2, 0, NULL, (PFCreateElement)(&DVR_Layout::InitMediaPlay)        , (PFOnEvent)(&DVR_Layout::OnPlayEvent), NULL},
        { "CGPUButton"     , "上一曲"     , 2, 0, NULL, (PFCreateElement)(&DVR_Layout::InitMediaNext)        , (PFOnEvent)(&DVR_Layout::OnNextEvent), NULL},
        { "CGPUButton"     , "下一曲"     , 2, 0, NULL, (PFCreateElement)(&DVR_Layout::InitMediaPrev)        , (PFOnEvent)(&DVR_Layout::OnPrevEvent), NULL},
        { "CGPUButton"     , "快进"       , 2, 0, NULL, (PFCreateElement)(&DVR_Layout::InitMediaForward)     , (PFOnEvent)(&DVR_Layout::OnForwardEvent), NULL},
        { "CGPUButton"     , "快退"       , 2, 0, NULL, (PFCreateElement)(&DVR_Layout::InitMediaRewind)      , (PFOnEvent)(&DVR_Layout::OnRewindEvent), NULL},
        { "CGPUText"       , "进度条时间" , 2, 0, NULL, (PFCreateElement)(&DVR_Layout::InitMediaText)        , NULL, NULL},
        { "CGPUButton"     , "退出"       , 2, 0, NULL, (PFCreateElement)(&DVR_Layout::InitMediaExit)        , (PFOnEvent)(&DVR_Layout::OnExitEvent), NULL},
        { "CGPUButton"     , "截图"       , 2, 0, NULL, (PFCreateElement)(&DVR_Layout::InitMediaScreenShot)  , (PFOnEvent)(&DVR_Layout::OnScreenShotEvent), NULL},
        { "CGPUButton"     , "设置"       , 2, 0, NULL, (PFCreateElement)(&DVR_Layout::InitMediaSetting)     , (PFOnEvent)(&DVR_Layout::OnSettingEvent), NULL},
        { "CGPUButton"    , "播放列表按钮", 2, 0, NULL, (PFCreateElement)(&DVR_Layout::InitMediaListviewPop),(PFOnEvent)(&DVR_Layout::OnListviewPop), NULL},
        { "CGPUProcessbar" , "进度条"     , 1, 0, NULL, (PFCreateElement)(&DVR_Layout::InitMediaBar)         , (PFOnEvent)(&DVR_Layout::OnBarEvent), NULL},
        { "CGPUListView"   , "播放列表"   , 1, 0, NULL, (PFCreateElement)(&DVR_Layout::InitMediaListview),(PFOnEvent)(&DVR_Layout::OnListviewEvent), NULL},
        //{ "CGPUImageStream", "播放列表缩略图", 2, 0, NULL, (PFCreateElement)(&DVR_Layout::InitMediaListviewThumbnail), NULL, NULL},
        { "CGPUButton", "播放列表下翻" , 2, 0, NULL, (PFCreateElement)(&DVR_Layout::InitMediaListviewPrev), (PFOnEvent)(&DVR_Layout::OnListviewPrevEvent), NULL},
        { "CGPUButton", "播放列表上翻" , 2, 0, NULL, (PFCreateElement)(&DVR_Layout::InitMediaListviewNext), (PFOnEvent)(&DVR_Layout::OnListviewNextEvent), NULL},
        { "CGPUButton", "播放列表确定" , 2, 0, NULL, (PFCreateElement)(&DVR_Layout::InitMediaListviewOk), (PFOnEvent)(&DVR_Layout::OnListviewOkEvent), NULL},
        //{ "CGPUIcon"       , "指示灯"     , 1, 0, NULL, (PFCreateElement)(&DVR_Layout::InitMediaStateIcon), NULL, NULL},
    };
    
    DVR_Layout::DVR_Layout()
        :ILayout()
        ,m_element_size(sizeof(m_element_info) / sizeof(struct ElementFuntionTable))
    {
        InitElementTable(m_element_info, m_element_size);
    }

    DVR_Layout::~DVR_Layout()
    {
    }
    
    void DVR_Layout::Enable(bool flag)
    {
        ILayout::EnableLayout(flag);
    }
    void DVR_Layout::SetProcessbarValue(uint32_t whole_time, uint32_t cur_time)
    {
        m_bar->SetValue(whole_time, cur_time);

        whole_time /= 1000;
        cur_time   /= 1000;
        char time_text[32];
        sprintf(time_text, "%u-%u-%u / %u-%u-%u",
                cur_time / 3600, (cur_time / 60) % 60, cur_time % 60,
                whole_time / 3600,(whole_time / 60) % 60, whole_time % 60
                );
        
        m_bar_text->SetText(time_text);
    }
    void DVR_Layout::InsertProcessbarKeyFrame(void* frame)
    {
        
    }
    PlaylistItemTable_T* DVR_Layout::GetPlaylistItemTable()
    {
        return &table;
    }
    void DVR_Layout::SyncPlaylist()
    {
        m_listview->Reset();
        for(int index = 0; index < table.itemNum; index++)
        {
            m_listview->SetItemText(table.item[index].itemName, index);
        }
        m_listview->Sync();
    }
    void DVR_Layout::AppendPlaylist(const char* playlist)
    {
        
    }
    void DVR_Layout::NextItemInPlaylist()
    {
        static bool down = true;
        if(down)
        {
            OnMouseSingleDown(210, 589);
            down =false;
        }
        else
        {
            OnMouseSingleUp(210, 589);
            down = true;
        }
    }
    void DVR_Layout::PrevItemInPlaylist()
    {
        static bool down = true;
        if(down)
        {
            OnMouseSingleDown(20, 589);
            down =false;
        }
        else
        {
            OnMouseSingleUp(20, 589);
            down = true;
        }
    }
    /**
     * \brief 内部测试专用接口,  实际采用触摸屏左边进行Layout的坐标响应操作
     */
    void DVR_Layout::StartPlay()
    {
        OnMouseSingleDown(640, 670);
    }
    void DVR_Layout::PausePlay()
    {
        OnMouseSingleDown(640, 670);
    }
    void DVR_Layout::NextPlay()
    {
        OnMouseSingleDown(690, 670);
    }
    void DVR_Layout::PrevPlay()
    {
        OnMouseSingleDown(526, 670);
    }
    void DVR_Layout::FastForwardPlay()
    {
        OnMouseSingleDown(456, 670);
    }
    void DVR_Layout::RewindPlay()
    {
        OnMouseSingleDown(760, 670);
    }
    void DVR_Layout::SetPlaylist()
    {
        static bool down = true;
        if(down)
        {
            OnMouseSingleDown(126, 589);
            down =false;
        }
        else
        {
            OnMouseSingleUp(126, 589);
            down = true;
        }
    }
    /**
     * \brief DVR 控件元素操作接口实现，　目前为手动添加并实现，后期可通过UI设计并自动代码生成并添加
     *　　　　主要涉及的是UI的绘制代码，private 不对外暴露实现细节
     */
    static IGUITexture panel_array_texture[] = {
        {XR_RES_DVR"panel.dds", 0, 620, 1280, 100},
    };
    static IGUITexture prev_array_texture[] =  {
        {XR_RES_DVR"media_prev.dds", 526, 26, 64, 48},
        {XR_RES_DVR"media_prev_rewind_hit.dds", 526, 26, 64, 48},
    };
    static IGUITexture play_array_texture[] =  {
        {XR_RES_DVR"media_pause.dds", 590, 0, 100, 100},
        {XR_RES_DVR"media_play.dds", 590, 0, 100, 100}
    };
    static IGUITexture next_array_texture[] = {
        {XR_RES_DVR"media_next.dds", 690, 26, 64, 48},
        {XR_RES_DVR"media_next_forward_hit.dds", 690, 26, 64, 48}
    };
    static IGUITexture forward_array_texture[] = {
        {XR_RES_DVR"media_forward.dds", 824, 26, 64, 48},
        {XR_RES_DVR"media_next_forward_hit.dds", 824, 26, 64, 48}
    };
    static IGUITexture rewind_array_texture[] = {
        {XR_RES_DVR"media_rewind.dds", 760, 26, 64, 48},
        {XR_RES_DVR"media_prev_rewind_hit.dds", 760, 26, 64, 48}
    };
    static IGUITexture bar_array_texture[] = {
        {XR_RES_DVR"barBase.dds", 0, 602, 1280, 18},
        {XR_RES_DVR"barSlide.dds", 0, 602, 1280, 18}
    };
    static IGUITexture exit_array_texture[] = {
        {XR_RES_DVR"media_on.dds",  1200, 13, 74, 74},
        {XR_RES_DVR"media_off.dds", 1200, 13, 74, 74}
    };
    static IGUITexture text_array_texture[] = {
        {XR_RES_DVR"BC64.dds", 240, 30, 80, 40},
        {XR_RES"text_box.ttf", 1200, 10, 80, 80}
    };
    static IGUITexture listviewpop_array_texture[] =
    {
        {XR_RES_DVR"media_listview_pop.dds",   24, 25, 48, 50},
        {XR_RES_DVR"media_listview_poped.dds", 24, 25, 48, 50}
    };
    static IGUITexture listview_array_texture[] = {
       {XR_RES_DVR"media_listview_bg.dds", 0, 200, 602, 350},
       {XR_RES_DVR"media_listview_itemPlay.dds",0, 33, 329, 35},
       {XR_RES_DVR"media_listview_item.dds", 0, 68, 329, 35},
       {XR_RES_DVR"media_listview_itemSelected.dds",0, 68, 329, 35},
       {XR_RES"text_box.ttf", 0, 0, 0, 0}
    };
    static IGUITexture listview_pageprev_texture[] =
    {
        {XR_RES_DVR"media_listview_itemPrev.dds", 25, 0, 33, 33},
        {XR_RES_DVR"BC64.dds", 25, 0, 33, 33},
    };
    static IGUITexture listview_itemok_texture[] =
    {
        {XR_RES_DVR"media_listview_itemOk.dds", 147, 0, 33, 33},
        {XR_RES_DVR"BC64.dds", 147, 0, 33, 33},
    };
    static IGUITexture listview_pagenext_texture[] =
    {
        {XR_RES_DVR"media_listview_itemNext.dds", 270, 0, 33, 33},
        {XR_RES_DVR"BC64.dds", 270, 0, 33, 33},
    };
    static IGUITexture listviewThumbnail_array_texture[] = {
        {XR_RES_DVR"media_listview_thumbnail.dds", 329, 0, 273, 348},
    };
    void DVR_Layout::InitMediaPanel(IGUIElement* media_panel, const GUI_HANDLE_T parentId)
    {
        media_panel->Attach(m_node, parentId);
        media_panel->SetTexture(panel_array_texture, 0);
        media_panel->Create(panel_array_texture[0].pos_x,
                            panel_array_texture[0].pos_y,
                            panel_array_texture[0].element_width,
                            panel_array_texture[0].element_height);
    }
    void DVR_Layout::InitMediaPrev(IGUIElement* media_prev_button, const GUI_HANDLE_T parentId)
    {
        media_prev_button->Attach(m_node, parentId);
        media_prev_button->SetTexture(prev_array_texture, 0);
        media_prev_button->Create(prev_array_texture[0].pos_x,
                                  prev_array_texture[0].pos_y,
                                  prev_array_texture[0].element_width,
                                  prev_array_texture[0].element_height);
    }
    void DVR_Layout::InitMediaPlay(IGUIElement* media_play_button, const GUI_HANDLE_T parentId)
    {
        media_play_button->Attach(m_node, parentId);
        media_play_button->SetTexture(play_array_texture, GUI::GUI_BUTTON_EFFECT_LOCK);
        media_play_button->Create(play_array_texture[0].pos_x,
                                  play_array_texture[0].pos_y,
                                  play_array_texture[0].element_width,
                                  play_array_texture[0].element_height);
        IGUIElement* button = media_play_button;
        m_media_play = dynamic_cast<CGPUButton*>(button);
    }
    void DVR_Layout::InitMediaNext(IGUIElement* media_next_button, const GUI_HANDLE_T parentId)
    {
        media_next_button->Attach(m_node, parentId);
        media_next_button->SetTexture(next_array_texture, 0);
        media_next_button->Create(next_array_texture[0].pos_x,
                                  next_array_texture[0].pos_y,
                                  next_array_texture[0].element_width,
                                  next_array_texture[0].element_height);
    }
    void DVR_Layout::InitMediaForward(IGUIElement* media_forward_button, const GUI_HANDLE_T parentId)
    {

        media_forward_button->Attach(m_node, parentId);
        media_forward_button->SetTexture(forward_array_texture, 0);
        media_forward_button->Create(forward_array_texture[0].pos_x,
                                     forward_array_texture[0].pos_y,
                                     forward_array_texture[0].element_width,
                                     forward_array_texture[0].element_height);
    }
    void DVR_Layout::InitMediaRewind(IGUIElement* media_rewind_button, const GUI_HANDLE_T parentId)
    {
        
        media_rewind_button->Attach(m_node, parentId);
        media_rewind_button->SetTexture(rewind_array_texture, 0);
        media_rewind_button->Create(rewind_array_texture[0].pos_x,
                                    rewind_array_texture[0].pos_y,
                                    rewind_array_texture[0].element_width,
                                    rewind_array_texture[0].element_height);
    }
    void DVR_Layout::InitMediaBar(IGUIElement* media_bar, const GUI_HANDLE_T parentId)
    {
        media_bar->Attach(m_node, parentId);
        media_bar->SetTexture(bar_array_texture, 0);
        media_bar->Create(bar_array_texture[0].pos_x,
                          bar_array_texture[0].pos_y,
                          bar_array_texture[0].element_width,
                          bar_array_texture[0].element_height);
        m_bar = dynamic_cast<CGPUProcessbar*>(media_bar);
    }
    void DVR_Layout::InitMediaText(IGUIElement* media_text, const GUI_HANDLE_T parentId)
    {
        media_text->Attach(m_node, parentId);
        media_text->SetTexture(text_array_texture, 0);
        media_text->Create(text_array_texture[0].pos_x,
                           text_array_texture[0].pos_y,
                           text_array_texture[0].element_width,
                           text_array_texture[0].element_height);
        media_text -> SetText("/");
        m_bar_text = dynamic_cast<CGPUText*>(media_text);
    }
    
    void DVR_Layout::InitMediaListview(IGUIElement* media_listview, const GUI_HANDLE_T parentId)
    {
        media_listview->Attach(m_node, parentId);
        media_listview->SetTexture(listview_array_texture, 0);
        media_listview->Create(listview_array_texture[0].pos_x,
                               listview_array_texture[0].pos_y,
                               listview_array_texture[0].element_width,
                               listview_array_texture[0].element_height);
        
        m_listview = dynamic_cast<CGPUListView*>(media_listview);
    }
    void DVR_Layout::InitMediaStateIcon(IGUIElement*, const GUI_HANDLE_T parentId)
    {
        
    }
    void DVR_Layout::InitMediaSetting(IGUIElement* media_setting, const GUI_HANDLE_T parentId)
    {
        
    }
    void DVR_Layout::InitMediaScreenShot(IGUIElement* media_screenshot, const GUI_HANDLE_T parentId)
    {
        
    }
    void DVR_Layout::InitMediaExit(IGUIElement* media_exit, const GUI_HANDLE_T parentId)
    {
        
        media_exit->Attach(m_node, parentId);
        media_exit->SetTexture(exit_array_texture, 0);
        media_exit->Create(exit_array_texture[0].pos_x,
                           exit_array_texture[0].pos_y,
                           exit_array_texture[0].element_width,
                           exit_array_texture[0].element_height);
    }
    void DVR_Layout::InitMediaListviewPop(IGUIElement* media_listview_pop_button, const GUI_HANDLE_T parentId)
    {
        media_listview_pop_button->Attach(m_node, parentId);
        media_listview_pop_button->SetTexture(listviewpop_array_texture, GUI::GUI_BUTTON_EFFECT_LOCK);
        media_listview_pop_button->Create(listviewpop_array_texture[0].pos_x,
                                          listviewpop_array_texture[0].pos_y,
                                          listviewpop_array_texture[0].element_width,
                                          listviewpop_array_texture[0].element_height);
    }
    void DVR_Layout::InitMediaListviewThumbnail(IGUIElement* media_thumbnail, const GUI_HANDLE_T parentId)
    {
        media_thumbnail->Attach(m_node, parentId);
        media_thumbnail->SetTexture(listviewThumbnail_array_texture, 0);
        media_thumbnail->Create(listviewThumbnail_array_texture[0].pos_x,
                                listviewThumbnail_array_texture[0].pos_y,
                                listviewThumbnail_array_texture[0].element_width,
                                listviewThumbnail_array_texture[0].element_height);
        m_listview_thumbnail = dynamic_cast<CGPUImageStream*>(media_thumbnail);
        //初始化缩略图地址
        for( int index = 0; index < table.itemNum; index ++)
        {
            table.item[index].addr.itemThumbnailAddr =
                m_listview_thumbnail->GetImageRawData(&table.item[index].thumbnail_width,
                                                      &table.item[index].thumbnail_height);
        }
    }
    void DVR_Layout::InitMediaListviewPrev(IGUIElement* listviewPrev_button, const GUI_HANDLE_T parentId)
    {
        listviewPrev_button->Attach(m_node, parentId);
        listviewPrev_button->SetTexture(listview_pageprev_texture, 0);
        listviewPrev_button->Create(listview_pageprev_texture[0].pos_x,
                                    listview_pageprev_texture[0].pos_y,
                                    listview_pageprev_texture[0].element_width,
                                    listview_pageprev_texture[0].element_height);
    }
    void DVR_Layout::InitMediaListviewNext(IGUIElement* listviewNext_button, const GUI_HANDLE_T parentId)
    {
        listviewNext_button->Attach(m_node, parentId);
        listviewNext_button->SetTexture(listview_pagenext_texture, 0);
        listviewNext_button->Create(listview_pagenext_texture[0].pos_x,
                                    listview_pagenext_texture[0].pos_y,
                                    listview_pagenext_texture[0].element_width,
                                    listview_pagenext_texture[0].element_height);
    }
    void DVR_Layout::InitMediaListviewOk(IGUIElement* listviewOk_button, const GUI_HANDLE_T parentId)
    {
        listviewOk_button->Attach(m_node, parentId);
        listviewOk_button->SetTexture(listview_itemok_texture, 0);
        listviewOk_button->Create(listview_itemok_texture[0].pos_x,
                                  listview_itemok_texture[0].pos_y,
                                  listview_itemok_texture[0].element_width,
                                  listview_itemok_texture[0].element_height);
    }
    /**
     *  \brief DVR 控件元素事件响应
     */
    void DVR_Layout::OnPlayEvent(IGUIElement* play_button)
    {
        void* payload = NULL;
        AvmEvent* event = RequestEvent(&payload);
        DVR_Event_Payload_T* data = (DVR_Event_Payload_T*)(payload);
        //填充有效数据
        data->header.msg_id = DVR_MEDIA_PLAY_BUTTON;
        data->body.onlyNotify = true;
        PostEvent(event);
    }
    void DVR_Layout::OnNextEvent(IGUIElement* next_button)
    {
        m_media_play->Reset();
        void* payload = NULL;
        AvmEvent* event = RequestEvent(&payload);
        DVR_Event_Payload_T* data = (DVR_Event_Payload_T*)(payload);

        if(m_listview->NextItem())
        {
            //填充有效数据
            data->header.msg_id = DVR_MEDIA_NEXT_BUTTON;
            data->body.onlyNotify = true;
        }
        else
        {
            //!列表框下移越界，触发向下翻页命令
            data->header.msg_id = DVR_MEDIA_LIST_VIEW;
            data->body.listview_file.operation = 0x06;
            data->body.listview_file.method.file_play = m_listview->GetCurrentIndex();
        }
        PostEvent(event);
    }
    void DVR_Layout::OnPrevEvent(IGUIElement* prev_button)
    {
        m_media_play->Reset();
        void* payload = NULL;
        AvmEvent* event = RequestEvent(&payload);
        DVR_Event_Payload_T* data = (DVR_Event_Payload_T*)(payload);
        
        if(m_listview->PrevItem()) 
        {
            //填充有效数据
            data->header.msg_id = DVR_MEDIA_PREVE_BUTTON;
            data->body.onlyNotify = true;
        }
        else
        {
            //!列表框上移越界，触发向上翻页命令
            data->header.msg_id = DVR_MEDIA_LIST_VIEW;
            data->body.listview_file.operation = 0x05;
            data->body.listview_file.method.file_play = m_listview->GetCurrentIndex();
        }
        PostEvent(event);
    }
    void DVR_Layout::OnForwardEvent(IGUIElement* forward_button)
    {
        void* payload = NULL;
        AvmEvent* event = RequestEvent(&payload);
        DVR_Event_Payload_T* data = (DVR_Event_Payload_T*)(payload);
        //填充有效数据
        data->header.msg_id = DVR_MEDIA_FORWARD_BUTTON;
        data->body.onlyNotify = true;
        PostEvent(event);
    }
    void DVR_Layout::OnRewindEvent(IGUIElement* rewind_button)
    {
        void* payload = NULL;
        AvmEvent* event = RequestEvent(&payload);
        DVR_Event_Payload_T* data = (DVR_Event_Payload_T*)(payload);
        //填充有效数据
        data->header.msg_id = DVR_MEDIA_REWIND_BUTTON;
        data->body.onlyNotify = true;
        PostEvent(event);
    }
    void DVR_Layout::OnScreenShotEvent(IGUIElement* screenshot_button)
    {
        void* payload = NULL;
        AvmEvent* event = RequestEvent(&payload);
        DVR_Event_Payload_T* data = (DVR_Event_Payload_T*)(payload);
        //填充有效数据
        data->header.msg_id = DVR_MEDIA_SCREEN_SHOT_BUTTON;
        data->body.onlyNotify = true;
        PostEvent(event);
    }
    void DVR_Layout::OnSettingEvent(IGUIElement* setting_button)
    {
        void* payload = NULL;
        AvmEvent* event = RequestEvent(&payload);
        DVR_Event_Payload_T* data = (DVR_Event_Payload_T*)(payload);
        //填充有效数据
        data->header.msg_id = DVR_MEDIA_SETTING_BUTTON;
        data->body.onlyNotify = true;
        PostEvent(event);
    }
    void DVR_Layout::OnBarEvent(IGUIElement* bar_button)
    {
        void* payload = NULL;
        AvmEvent* event = RequestEvent(&payload);
        DVR_Event_Payload_T* data = (DVR_Event_Payload_T*)(payload);
        //填充有效数据
        data->header.msg_id = DVR_MEDIA_BAR;
        data->body.bar.operation = 0x01;
        data->body.bar.method.jump_time = dynamic_cast<CGPUProcessbar*>(bar_button)->GetPos();
        PostEvent(event);
    }
    void DVR_Layout::OnExitEvent(IGUIElement* exit_button)
    {
        m_media_play->Reset();
        void* payload = NULL;
        AvmEvent* event = RequestEvent(&payload);
        DVR_Event_Payload_T* data = (DVR_Event_Payload_T*)(payload);
        //填充有效数据
        data->header.msg_id = DVR_MEDIA_EXIT_BUTTON;
        data->body.onlyNotify = true;
        PostEvent(event);
    }
    void DVR_Layout::OnListviewEvent(IGUIElement* list_view)
    {
        //更新缩略图
        //m_listview_thumbnail->UpdateImage();
        
        void* payload = NULL;
        AvmEvent* event = RequestEvent(&payload);
        DVR_Event_Payload_T* data = (DVR_Event_Payload_T*)(payload);
        //填充有效数据
        data->header.msg_id = DVR_MEDIA_LIST_VIEW;
        data->body.listview_file.operation = 0x04;
        data->body.listview_file.method.file_play = dynamic_cast<CGPUListView*>(list_view)->GetCurrentIndex();
        PostEvent(event);
    }
    void DVR_Layout::OnListviewPrevEvent(IGUIElement* listviewPrev_button)
    {
        void* payload = NULL;
        AvmEvent* event = RequestEvent(&payload);
        DVR_Event_Payload_T* data = (DVR_Event_Payload_T*)(payload);
        //填充有效数据
        data->header.msg_id = DVR_MEDIA_LIST_VIEW;
        data->body.listview_file.operation = 0x05;
        data->body.listview_file.method.file_play = m_listview->GetCurrentIndex();
        PostEvent(event);
    }
    void DVR_Layout::OnListviewNextEvent(IGUIElement* listviewNext_button)
    {
        void* payload = NULL;
        AvmEvent* event = RequestEvent(&payload);
        DVR_Event_Payload_T* data = (DVR_Event_Payload_T*)(payload);
        //填充有效数据
        data->header.msg_id = DVR_MEDIA_LIST_VIEW;
        data->body.listview_file.operation = 0x06;
        data->body.listview_file.method.file_play = m_listview->GetCurrentIndex();
        PostEvent(event);
    }
    void DVR_Layout::OnListviewOkEvent(IGUIElement* listviewOk_button)
    {
        void* payload = NULL;
        AvmEvent* event = RequestEvent(&payload);
        DVR_Event_Payload_T* data = (DVR_Event_Payload_T*)(payload);
        //填充有效数据
        data->header.msg_id = DVR_MEDIA_BAR;
        data->body.listview_file.operation = 0x05;
        data->body.listview_file.method.file_play = m_listview->GetCurrentIndex();
        PostEvent(event);
    }
    void DVR_Layout::OnListviewPop(IGUIElement* listviewpop_button)
    {
        static bool flag = false;
        m_listview->Enable(flag);
        flag = !flag;
    }
};

/**
 * 接口函数，操作Dvr_Layout
 */
extern "C" DLL_PUBLIC IDVR* NewDvrLayout()
{
    return(GUI::DVR_Layout::GetLayout());
}

extern "C" DLL_PUBLIC void DeleteDvrLayout(IDVR* dvr)
{
    if(dvr)
    {
        delete dvr;
    }
}
/*------------------------------------------------------------------------------------------
 * File Revision History (top to bottom: first revision to last revision)
 *==========================================================================================
 *
 * Date             SCR                      user                              Description
 *==========================================================================================

 *------------------------------------------------------------------------------------------*/