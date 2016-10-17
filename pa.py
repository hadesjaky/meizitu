#!/usr/bin/python3.4
# -*- coding: utf-8 -*-
"""
__title__ = '妹子图爬虫'
__author__ = 'hades'
__mtime__ = '16-10-12'
             ┏┓   ┏┓
            ┏┛┻━━━┛┻┓
            ┃   ☃   ┃
            ┃ ┳┛ ┗┳ ┃
            ┃   ┻   ┃
            ┗━┓   ┏━┛
             ┃    ┗━━━━┓
             ┃ 神兽保佑  ┣┓
             ┃永无BUG！┏┛
             ┗┓┓┏━━┳┓┏┛
              ┃┫┫  ┃┫┫
              ┗┻┛  ┗┻┛
    功能：爬取http://www.meizitu.com
    http://meizitu.com/a/list_1_1.html 的图片
"""

#from spider import SpiderHTML
import sys, os, re, codecs, urllib, http
from urllib import request
from bs4 import BeautifulSoup

class MeiZiTu(object):
    def __init__(self, topUrl):
        self.__url = topUrl
        self._bs4 = self.getBs4(topUrl)
        self._girlInfo = []
    #保存文本内容到本地
    def saveText(self,filename,content,mode='w'):
        with codecs.open(filename, encoding='utf-8', mode=mode) as f:
            f.write(content)

    #获得BS4结构
    def getBs4(self, url, coding='gbk'):
        req = request.Request(url)
        req.add_header('User-Agent',
            'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.157 UBrowser/5.5.9703.2 Safari/537.36')
        with request.urlopen(req) as response:
            return BeautifulSoup(response.read().decode(coding))
    #开始爬
    def start(self):
        main_contents = self._bs4.find_all('', attrs={'class':'pic'})
        for girl in main_contents:
            girlInfo = {}
            tmp_name = re.findall('img alt="(.*?)" src=', str(girl))
            tmp_url = re.findall('href="(.*?)" target="_blank"', str(girl))
            tmpG = re.sub('&lt;b&gt;','',tmp_name[0])
            girlInfo['name'] = re.sub('&lt;/b&gt;', '', tmpG)
            girlInfo['url'] = tmp_url[0]
            self._girlInfo.append(girlInfo)#保存起来
            tmpDir = os.path.join('./', girlInfo['name'])
            if (os.path.exists(tmpDir)):
                print('已经获得过信息，去找下一位')
                continue
            print('进入--%s--' % girlInfo['name'] )
            self.sonStart(girlInfo['url'], tmpDir)#处理子路径
            #break

    #爬子路径,保存成txt文件
    def sonStart(self, url, dir):
        tmp_son_url = self.getBs4(url)
        son_contents = tmp_son_url.find_all('div', attrs={'id':'picture'})
        girlList = []

        tmp_name = re.findall('img alt="(.*?)" src=', str(son_contents))
        tmp_url = re.findall('src="(.*?)"/><br/>', str(son_contents))
        for num in range(0, len(tmp_name)):
            girlIn = {}
            girlIn['name'] = tmp_name[num].replace('"','')
            girlIn['url'] = tmp_url[num].replace('"', '')
            girlList.append(girlIn)
        meiNv = str()
        for num in girlList:
            meiNv += num['name']+ '|' + num['url'] + '\n'
            #print(meiNv)
            #保存
        if os.path.exists('url.txt'):
            self.saveText(os.path.join('./','url.txt'), meiNv, 'a')
        else:
            self.saveText(os.path.join('./','url.txt'), meiNv, 'w')

    #保存图片url
if __name__ == '__main__':
    pageStart, pageEnd, paramsNum = 1, 0, len(sys.argv)
    # 不输入参数
    # 1个参数爬取该页
    # 2个参数以上爬取范围
    listUrl = 'http://meizitu.com/a/list_1_'
    if paramsNum > 2:
        pageStart, pageEnd = sys.argv[1], sys.argv[2]
    elif paramsNum == 1:
        pageStart, pageEnd = str(1), 0
    else:
        pageStart, pageEnd = sys.argv[1], 0

    if int(pageEnd) < int(pageStart):
        pageEnd = pageStart
    #页数小于0处理
    if pageStart < str(0):
        pageStart = str(0)
    elif pageEnd < str(0):
       pageEnd = str(0)


    for page in range(int(pageStart), int(pageEnd)+1):
        URL = listUrl + str(page)+'.html'
        #print(URL)
        spider = MeiZiTu(URL)
        spider.start()






