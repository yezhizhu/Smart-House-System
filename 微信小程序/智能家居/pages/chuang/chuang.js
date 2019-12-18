Page({
  data: {
    state: 1,
    shu1: 0,
    shu2: 1
  },
  dakai: function () {
    this.setData({
      state: this.data.shu1
    })
  },
  guanbi: function () {
    this.setData({
      state: this.data.shu2
    })
  }
})