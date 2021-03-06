// CAudioChatroomDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "starrtcdemo.h"
#include "CAudioChatroomDlg.h"
#include "afxdialogex.h"
#include "CCreateLiveDialog.h"
#include "CInterfaceUrls.h"
// CAudioChatroomDlg 对话框

IMPLEMENT_DYNAMIC(CAudioChatroomDlg, CDialogEx)

CAudioChatroomDlg::CAudioChatroomDlg(CUserManager* pUserManager, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_AUDIO_CHATROOM, pParent)
{
	m_pUserManager = pUserManager;
	XHLiveManager::addChatroomGetListListener(this);
	m_pLiveManager = new XHLiveManager(this);
	m_pSoundManager = new CSoundManager(this);
	m_pLiveManager->setRtcMediaType(LIVE_MEDIA_TYPE_AUDIO_ONLY);
	m_pCurrentLive = NULL;
}

CAudioChatroomDlg::~CAudioChatroomDlg()
{
	if (m_pSoundManager != NULL)
	{
		delete m_pSoundManager;
		m_pSoundManager = NULL;
	}

	if (m_pLiveManager != NULL)
	{
		delete m_pLiveManager;
		m_pLiveManager = NULL;
	}
	if (m_pCurrentLive != NULL)
	{
		delete m_pCurrentLive;
		m_pCurrentLive = NULL;
	}
	mVLivePrograms.clear();
}

void CAudioChatroomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTCONTROL_AUDIO_CHATROOM_LIST, m_List);
	DDX_Control(pDX, IDC_STATIC_PIC1, m_Picture1);
	DDX_Control(pDX, IDC_STATIC_PIC2, m_Picture2);
	DDX_Control(pDX, IDC_STATIC_PIC3, m_Picture3);
	DDX_Control(pDX, IDC_STATIC_PIC4, m_Picture4);
	DDX_Control(pDX, IDC_STATIC_PIC5, m_Picture5);
	DDX_Control(pDX, IDC_STATIC_PIC6, m_Picture6);
	DDX_Control(pDX, IDC_STATIC_PIC7, m_Picture7);
}


BEGIN_MESSAGE_MAP(CAudioChatroomDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_AUDIO_CHATROOM_LISTBRUSH, &CAudioChatroomDlg::OnBnClickedButtonAudioChatroomListbrush)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_AUDIO_CHATROOM, &CAudioChatroomDlg::OnBnClickedButtonCreateAudioChatroom)
	ON_NOTIFY(NM_CLICK, IDC_LISTCONTROL_AUDIO_CHATROOM_LIST, &CAudioChatroomDlg::OnNMClickListcontrolAudioChatroomList)
END_MESSAGE_MAP()


// CAudioChatroomDlg 消息处理程序


void CAudioChatroomDlg::OnBnClickedButtonAudioChatroomListbrush()
{
	getLiveList();
}


void CAudioChatroomDlg::OnBnClickedButtonCreateAudioChatroom()
{
	//m_ApplyUploadButton.EnableWindow(FALSE);
	//m_ExitUpload.EnableWindow(FALSE);

	//if (m_pDataShowView != NULL)
	//{
	//	m_pDataShowView->removeAllUser();
	//	m_pDataShowView->setShowPictures();
	//}

	if (m_pSoundManager != NULL)
	{
		m_pSoundManager->stopGetSoundData();
	}
	if (m_pCurrentLive != NULL)
	{
		delete m_pCurrentLive;
		m_pCurrentLive = NULL;
	}

	CString strName = "";
	bool bPublic = false;
	CHATROOM_TYPE chatRoomType = CHATROOM_TYPE::CHATROOM_TYPE_PUBLIC;
	LIVE_VIDEO_TYPE channelType = LIVE_VIDEO_TYPE::LIVE_VIDEO_TYPE_LOGIN_SPECIFY;
	CCreateLiveDialog dlg(m_pUserManager);
	if (dlg.DoModal() == IDOK)
	{
		strName = dlg.m_strLiveName;
		bPublic = dlg.m_bPublic;
	}
	else
	{
		return;
	}
	if (m_pLiveManager != NULL)
	{
		string strLiveId = m_pLiveManager->createLive(strName.GetBuffer(0), chatRoomType, channelType);
		if (strLiveId != "")
		{
			//if (m_pUserManager->m_bUserDispatch)
			{
				CInterfaceUrls::demoReportAudioLive(strLiveId, strName.GetBuffer(0), m_pUserManager->m_ServiceParam.m_strUserId, m_pUserManager);
			}

			bool bRet = m_pLiveManager->startLive(strLiveId);
			if (bRet)
			{
				if (m_pCurrentLive == NULL)
				{
					m_pCurrentLive = new CLiveProgram();
				}
				m_pCurrentLive->m_strName = strName;
				m_pCurrentLive->m_strCreator = m_pUserManager->m_ServiceParam.m_strUserId.c_str();
				if (m_pSoundManager != NULL)
				{
					m_pSoundManager->startGetSoundData(true);
				}
			}
			getLiveList();
		}
		else
		{
			AfxMessageBox("创建直播失败!");
		}
	}
}


void CAudioChatroomDlg::OnNMClickListcontrolAudioChatroomList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	/*m_ApplyUploadButton.EnableWindow(FALSE);
	m_ExitUpload.EnableWindow(FALSE);
	if (m_pDataShowView != NULL)
	{
		m_pDataShowView->removeAllUser();
		m_pDataShowView->setShowPictures();
	}*/
	if (m_pSoundManager != NULL)
	{
		m_pSoundManager->stopGetSoundData();
	}
	if (m_pCurrentLive != NULL)
	{
		delete m_pCurrentLive;
		m_pCurrentLive = NULL;
	}

	int nItem = -1;
	if (pNMItemActivate != NULL)
	{
		nItem = pNMItemActivate->iItem;
		if (nItem >= 0)
		{
			CString strId = m_List.GetItemText(nItem, LIVE_VIDEO_ID);
			CString strName = m_List.GetItemText(nItem, LIVE_VIDEO_NAME);
			CString strCreater = m_List.GetItemText(nItem, LIVE_VIDEO_CREATER);

			CLiveProgram* pLiveProgram = getLiveProgram(strId, strName, strCreater);
			CString strUserId = m_pUserManager->m_ServiceParam.m_strUserId.c_str();
			if (pLiveProgram != NULL)
			{
				//if (/*pLiveProgram->m_liveState == false && */pLiveProgram->m_strCreator != strUserId)
				//{
				//	AfxMessageBox("直播尚未开始");
				//	return;
				//}
				string strId = pLiveProgram->m_strId.GetBuffer(0);
				if (strId.length() == 32)
				{
					if (pLiveProgram->m_strCreator == strUserId)
					{
						bool bRet = m_pLiveManager->startLive(strId);
						if (bRet)
						{
							if (m_pCurrentLive == NULL)
							{
								m_pCurrentLive = new CLiveProgram();
							}
							m_pCurrentLive->m_strId = pLiveProgram->m_strId;
							m_pCurrentLive->m_strName = pLiveProgram->m_strName;
							m_pCurrentLive->m_strCreator = pLiveProgram->m_strCreator;

							if (m_pSoundManager != NULL)
							{
								m_pSoundManager->startGetSoundData(true);
							}
						}
						else
						{
							AfxMessageBox("开启直播失败");
						}
					}
					else
					{
						bool bRet = m_pLiveManager->watchLive(strId);
						if (bRet)
						{
							//m_ApplyUploadButton.EnableWindow(TRUE);
							if (m_pCurrentLive == NULL)
							{
								m_pCurrentLive = new CLiveProgram();
							}
							m_pCurrentLive->m_strId = pLiveProgram->m_strId;
							m_pCurrentLive->m_strName = pLiveProgram->m_strName;
							m_pCurrentLive->m_strCreator = pLiveProgram->m_strCreator;
							if (m_pSoundManager != NULL)
							{
								m_pSoundManager->startGetSoundData(false);
							}
						}
						else
						{
							AfxMessageBox("观看直播失败");
						}
					}
				}
				else
				{
					CString strMessage = "";
					strMessage.Format("err id %s", strId.c_str());
					AfxMessageBox(strMessage);
				}
				delete pLiveProgram;
				pLiveProgram = NULL;
			}

		}
	}

}

/**
 * 查询聊天室列表回调
 */
int CAudioChatroomDlg::chatroomQueryAllListOK(list<ChatroomInfo>& listData)
{
	mVLivePrograms.clear();
	m_List.DeleteAllItems();

	list<ChatroomInfo>::iterator iter = listData.begin();
	int i = 0;
	for (; iter != listData.end(); iter++)
	{
		CLiveProgram liveProgram;
		liveProgram.m_strName = (char*)(*iter).m_strName.c_str();
		liveProgram.m_strId = (char*)(*iter).m_strRoomId.c_str();
		liveProgram.m_strCreator = (char*)(*iter).m_strCreaterId.c_str();
		mVLivePrograms.push_back(liveProgram);
	}
	int nRowIndex = 0;
	CString strStatus = "";
	for (int i = 0; i < (int)mVLivePrograms.size(); i++)
	{
		m_List.InsertItem(i, mVLivePrograms[i].m_strName);
		//m_liveList.AddString(mVLivePrograms[i].m_strName);
		m_List.SetItemText(i, LIVE_VIDEO_ID, mVLivePrograms[i].m_strId);

		m_List.SetItemText(i, LIVE_VIDEO_CREATER, mVLivePrograms[i].m_strCreator);
		if (mVLivePrograms[i].m_liveState)
		{
			strStatus = "正在直播";
		}
		else
		{
			strStatus = "直播未开始";
		}
		m_List.SetItemText(i, LIVE_VIDEO_STATUS, strStatus);
	}
	return 0;
}
/*
 * 获取音频聊天室列表
 */
void CAudioChatroomDlg::getLiveList()
{
	//if (m_pUserManager->m_bUserDispatch)
	{
		list<ChatroomInfo> listData;
		CInterfaceUrls::demoRequestAudioLiveList(listData, m_pUserManager);
		chatroomQueryAllListOK(listData);
	}
	//else
	//{
	//	XHLiveManager::getLiveList("", CHATROOM_LIST_TYPE_LIVE);
	//}
	
}


BOOL CAudioChatroomDlg::OnInitDialog()
{
	__super::OnInitDialog();

	LONG lStyle;
	lStyle = GetWindowLong(m_List.m_hWnd, GWL_STYLE);
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT;
	SetWindowLong(m_List.m_hWnd, GWL_STYLE, lStyle);

	DWORD dwStyleLiveList = m_List.GetExtendedStyle();
	dwStyleLiveList |= LVS_EX_FULLROWSELECT;                                        //选中某行使整行高亮(LVS_REPORT)
	dwStyleLiveList |= LVS_EX_GRIDLINES;                                            //网格线(LVS_REPORT)
	//dwStyle |= LVS_EX_CHECKBOXES;                                            //CheckBox
	m_List.SetExtendedStyle(dwStyleLiveList);

	m_List.InsertColumn(LIVE_VIDEO_ID, _T("ID"), LVCFMT_LEFT, 110);
	m_List.InsertColumn(LIVE_VIDEO_NAME, _T("Name"), LVCFMT_LEFT, 120);
	m_List.InsertColumn(LIVE_VIDEO_CREATER, _T("Creator"), LVCFMT_LEFT, 80);
	m_List.InsertColumn(LIVE_VIDEO_STATUS, _T("liveState"), LVCFMT_LEFT, 100);

	//HBITMAP hBitmap = LoadBitmap();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

/**
 * 有主播加入
 * @param liveID 直播ID
 * @param actorID 新加入的主播ID
 */
void CAudioChatroomDlg::onActorJoined(string liveID, string actorID)
{
	/*if (m_pDataShowView != NULL)
	{
		int nCount = m_pDataShowView->getUserCount();
		bool bBig = false;
		if (nCount == 0)
		{
			bBig = true;
		}
		m_pDataShowView->addUser(actorID, bBig);
		SetEvent(m_hSetShowPicEvent);
	}*/
}
/**
 * 有主播离开
 * @param liveID 直播ID
 * @param actorID 离开的主播ID
 */
void CAudioChatroomDlg::onActorLeft(string liveID, string actorID)
{
	/*if (m_pDataShowView != NULL)
	{
		m_pDataShowView->removeUser(actorID);
		m_pLiveManager->changeToSmall(actorID);
		string strUserId = "";
		bool bLeft = m_pDataShowView->isLeftOneUser(strUserId);
		if (bLeft && strUserId != "")
		{
			m_pLiveManager->changeToBig(strUserId);
			m_pDataShowView->changeShowStyle(strUserId, true);
		}
		SetEvent(m_hSetShowPicEvent);
	}*/
}

/**
 * 房主收到其他用户的连麦申请
 * @param liveID
 * @param applyUserID
 */
void CAudioChatroomDlg::onApplyBroadcast(string liveID, string applyUserID)
{
	CString strMsg;
	strMsg.Format("是否同意用户:%s申请连麦", applyUserID.c_str());
	if (m_pLiveManager != NULL)
	{
		if (IDYES == AfxMessageBox(strMsg, MB_YESNO))
		{
			m_pLiveManager->agreeApplyToBroadcaster(applyUserID);
			onActorJoined(liveID, applyUserID);
		}
		else
		{
			m_pLiveManager->refuseApplyToBroadcaster(applyUserID);
		}
	}
}

/**
 * 申请连麦用户收到的回复
 * @param liveID
 * @param result
 */
void CAudioChatroomDlg::onApplyResponsed(string liveID, bool bAgree)
{
	if (bAgree)
	{
		if (IDYES == AfxMessageBox("是否上传数据", MB_YESNO))
		{
			//m_ApplyUploadButton.EnableWindow(FALSE);
			//m_ExitUpload.EnableWindow(FALSE);

			//if (m_pDataShowView != NULL)
			//{
			//	m_pDataShowView->removeAllUser();
			//	m_pDataShowView->setShowPictures();
			//}

			if (m_pSoundManager != NULL)
			{
				m_pSoundManager->stopGetSoundData();
			}

			bool bRet = m_pLiveManager->startLive(m_pCurrentLive->m_strId.GetBuffer(0));
			if (bRet)
			{
				if (m_pSoundManager != NULL)
				{
					m_pSoundManager->startGetSoundData(true);
				}
				//m_ApplyUploadButton.EnableWindow(FALSE);
				//m_ExitUpload.EnableWindow(TRUE);
			}
			else
			{
				AfxMessageBox("join live failed");
			}
		}
	}
	else
	{
		AfxMessageBox("对方拒绝连麦请求");
	}
}

/**
* 普通用户收到连麦邀请
* @param liveID 直播ID
* @param applyUserID 发出邀请的人的ID（主播ID）
*/
void CAudioChatroomDlg::onInviteBroadcast(string liveID, string applyUserID)
{
}

/**
* 主播收到的邀请连麦结果
* @param liveID 直播ID
* @param result 邀请结果
*/
void CAudioChatroomDlg::onInviteResponsed(string liveID)
{
}

/**
 * 一些异常情况引起的出错，请在收到该回调后主动断开直播
 * @param liveID 直播ID
 * @param error 错误信息
 */
void CAudioChatroomDlg::onLiveError(string liveID, string error)
{
	//m_ApplyUploadButton.EnableWindow(FALSE);
	//m_ExitUpload.EnableWindow(FALSE);
	//if (m_pDataShowView != NULL)
	//{
	//	m_pDataShowView->removeAllUser();
	//	m_pDataShowView->setShowPictures();
	//}

	if (m_pSoundManager != NULL)
	{
		m_pSoundManager->stopGetSoundData();
	}
	if (m_pCurrentLive != NULL)
	{
		delete m_pCurrentLive;
		m_pCurrentLive = NULL;
	}
}

/**
 * 聊天室成员数变化
 * @param number
 */
void CAudioChatroomDlg::onMembersUpdated(int number)
{
}

/**
 * 自己被踢出聊天室
 */
void CAudioChatroomDlg::onSelfKicked()
{
}

/**
 * 自己被踢出聊天室
 */
void CAudioChatroomDlg::onSelfMuted(int seconds)
{
}

/**
* 连麦者的连麦被强制停止
* @param liveID 直播ID
*/
void CAudioChatroomDlg::onCommandToStopPlay(string  liveID)
{
	//m_ApplyUploadButton.EnableWindow(FALSE);
	//m_ExitUpload.EnableWindow(FALSE);

	//if (m_pDataShowView != NULL)
	//{
	//	m_pDataShowView->removeAllUser();
	//	m_pDataShowView->setShowPictures();
	//}

	if (m_pSoundManager != NULL)
	{
		m_pSoundManager->stopGetSoundData();
	}
	if (m_pLiveManager != NULL)
	{
		m_pLiveManager->leaveLive();
	}
	if (m_pCurrentLive != NULL)
	{
		delete m_pCurrentLive;
		m_pCurrentLive = NULL;
	}
}
/**
 * 收到消息
 * @param message
 */
void CAudioChatroomDlg::onReceivedMessage(CIMMessage* pMessage)
{
}

/**
 * 收到私信消息
 * @param message
 */
void CAudioChatroomDlg::onReceivePrivateMessage(CIMMessage* pMessage)
{
}


int CAudioChatroomDlg::getRealtimeData(string strUserId, uint8_t* data, int len)
{
	return 0;
}
int CAudioChatroomDlg::getVideoRaw(string strUserId, int w, int h, uint8_t* videoData, int videoDataLen)
{
	return 0;
}
void CAudioChatroomDlg::getLocalSoundData(char* pData, int nLength)
{
	if (m_pLiveManager != NULL)
	{
		m_pLiveManager->insertAudioRaw((uint8_t*)pData, nLength);
	}
}

void CAudioChatroomDlg::querySoundData(char** pData, int* nLength)
{
	if (m_pLiveManager != NULL)
	{
		m_pLiveManager->querySoundData((uint8_t**)pData, nLength);
	}
}

CLiveProgram* CAudioChatroomDlg::getLiveProgram(CString strId, CString strName, CString strCreator)
{
	CLiveProgram* pRet = NULL;
	for (int i = 0; i < (int)mVLivePrograms.size(); i++)
	{
		if (//mVLivePrograms[i].m_strId == strId &&
			mVLivePrograms[i].m_strName == strName)// &&
			//mVLivePrograms[i].m_strCreator == strCreator)
		{
			pRet = new CLiveProgram();
			pRet->m_strId = mVLivePrograms[i].m_strId;
			pRet->m_strName = mVLivePrograms[i].m_strName;
			pRet->m_strCreator = mVLivePrograms[i].m_strCreator;
			pRet->m_liveState = mVLivePrograms[i].m_liveState;
			break;
		}
	}
	return pRet;
}
