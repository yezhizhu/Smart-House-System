Page({
data:{
state:1,
shu1:0,
shu2:1
},
  dakai: function () {
   
    var that = this //创建一个名为that的变量来保存this当前的值  
    wx.request({
      url: 'http://api.heclouds.com/devices/575681988/datapoints?type=3',
      method: 'post',
      data: {
        kongtiao: '4'  //这里是发送给服务器的参数（参数名：参数值）  
      },
      header: {
        'content-type': 'application/json',
        'api-key': '=c=iJ1e3YuFG6LMj3KLz6B1QUZs='
      },
      success: function (res) {
        that.setData({ //这里是修改data的值  
          test: res.data //test等于服务器返回来的数据  
        });
        console.log(res.data)
      }
    })

    4; setTimeout(function () {
      var that = this //创建一个名为that的变量来保存this当前的值  
      var that = this //创建一个名为that的变量来保存this当前的值  
      wx.request({
        url: 'http://api.heclouds.com/devices/575681988/datapoints?type=3',
        method: 'post',
        data: {
          kongtiao: '0'  //这里是发送给服务器的参数（参数名：参数值）  
        },
        header: {
          'content-type': 'application/json',
          'api-key': '=c=iJ1e3YuFG6LMj3KLz6B1QUZs='
        },
        success: function (res) {
          that.setData({ //这里是修改data的值  
            test: res.data //test等于服务器返回来的数据  
          });
          console.log(res.data)
        }
      }) //你需要执行的代码
    }, 6000)
    this.setData({
      state: this.data.shu1
    })
},
  guanbi: function () {
    var that = this //创建一个名为that的变量来保存this当前的值  
    wx.request({
      url: 'http://api.heclouds.com/devices/575681988/datapoints?type=3',
      method: 'post',
      data: {
        kongtiao: '5'  //这里是发送给服务器的参数（参数名：参数值）  
      },
      header: {
        'content-type': 'application/json',
        'api-key': '=c=iJ1e3YuFG6LMj3KLz6B1QUZs='
      },
      success: function (res) {
        that.setData({ //这里是修改data的值  
          test: res.data //test等于服务器返回来的数据  
        });
        console.log(res.data)
      }
    })

    5; setTimeout(function () {
      var that = this //创建一个名为that的变量来保存this当前的值  
      wx.request({
        url: 'http://api.heclouds.com/devices/575681988/datapoints?type=3',
        method: 'post',
        data: {
          kongtiao: '0'  //这里是发送给服务器的参数（参数名：参数值）  
        },
        header: {
          'content-type': 'application/json',
          'api-key': '=c=iJ1e3YuFG6LMj3KLz6B1QUZs='
        },
        success: function (res) {
          that.setData({ //这里是修改data的值  
            test: res.data //test等于服务器返回来的数据  
          });
          console.log(res.data)
        }
      }) //你需要执行的代码
    }, 6000)
    this.setData({
      state: this.data.shu2
    })
  }
})
