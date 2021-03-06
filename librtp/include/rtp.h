#ifndef _rtp_h_
#define _rtp_h_

#ifdef __cplusplus
extern "C" {
#endif

enum { 
	RTCP_MSG_MEMBER,	/// new member(re-calculate RTCP Transmission Interval)
	RTCP_MSG_EXPIRED,	/// member leave(re-calculate RTCP Transmission Interval)
	RTCP_MSG_BYE,		/// member leave(re-calculate RTCP Transmission Interval)
	RTCP_MSG_APP, 
};

struct rtcp_msg_t
{
	int type;
	union rtcp_msg_u
	{
		// RTCP_MSG_MEMBER
		struct rtcp_member_t
		{
			unsigned int ssrc;
		} member;

		// RTCP_MSG_EXPIRED
		struct rtcp_expired_t
		{
			unsigned int ssrc;
		} expired;

		// RTCP_MSG_BYE
		struct rtcp_bye_t
		{
			unsigned int ssrc;
			void* reason;
			size_t bytes; // reason length
		} bye;

		// RTCP_MSG_APP
		struct rtcp_app_t
		{
			unsigned int ssrc;
			char name[4];
			void* data;
			size_t bytes; // data length
		} app;
	} u;
};

struct rtp_event_t
{
	void (*on_rtcp)(void* param, const struct rtcp_msg_t* msg);
};

/// @param[in] boundwidth in byte
void* rtp_create(struct rtp_event_t *handler, void* param, unsigned int ssrc, size_t frequence, size_t boundwidth);
int rtp_destroy(void* rtp);

/// RTP send notify
/// @param[in] rtp RTP object
/// @param[in] data RTP packet(include RTP Header)
/// @param[in] bytes RTP packet size in byte
/// @param[in] time UTC time (map rtp timestamp to wall-clock)
/// @return 0-ok
int rtp_onsend(void* rtp, const void* data, size_t bytes, time64_t time);

/// RTP receive notify
/// @param[in] rtp RTP object
/// @param[in] data RTP packet(include RTP Header)
/// @param[in] bytes RTP packet size in byte
/// @param[out] time UTC time
/// @return 0-ok, -1-failed(don't receive RTCP packet)
int rtp_onreceived(void* rtp, const void* data, size_t bytes, time64_t *time);

/// received RTCP packet
/// @param[in] rtp RTP object
/// @param[in] rtcp RTCP packet(include RTCP Header)
/// @param[in] bytes RTCP packet size in byte
/// @return 0-ok
int rtp_onreceived_rtcp(void* rtp, const void* rtcp, size_t bytes);

/// create RTCP Report(SR/RR) packet
/// @param[in] rtp RTP object
/// @param[out] rtcp RTCP packet(include RTCP Header)
/// @param[in] bytes RTCP packet size in byte
/// @return 0-ok
size_t rtp_rtcp_report(void* rtp, void* rtcp, size_t bytes);

size_t rtp_rtcp_bye(void* rtp, void* rtcp, size_t bytes);

int rtp_rtcp_interval(void* rtp);

const char* rtp_get_cname(void* rtp, unsigned int ssrc);
const char* rtp_get_name(void* rtp, unsigned int ssrc);
int rtp_set_info(void* rtp, const char* cname, const char* name);

#ifdef __cplusplus
}
#endif
#endif /* !_rtp_h_ */
