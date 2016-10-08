<%--
Created by IntelliJ IDEA.
User: zhenyu
Date: 13-2-1
Time: 下午3:46
To change this template use File | Settings | File Templates.
url, sid, cm(cmatch), idx(index) 四个值。
--%>
<%@ page language="java" import="java.util.*,org.apache.log4j.Logger,java.net.URLDecoder" pageEncoding="UTF-8"%>
<%!Logger log = Logger.getLogger("dsp");%>
<%
String args = request.getQueryString();
int idx = args.indexOf("dsp_click_url=");
if (-1 == idx) {

} else {

	String target_url = args.substring(idx + "dsp_click_url=".length());

	try{
		log.info(args);
		response.sendRedirect(target_url);
	}catch(Exception e){
		log.error("dsp Exception:"+args);
	}finally {

	}

}
