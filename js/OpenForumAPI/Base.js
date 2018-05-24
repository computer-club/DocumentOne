var PostInquiry=require('./PostInquiry.js');
var PostRequest=require('./PostRequest.js');
var UserRequest=require('./UserRequest.js');

module.exports = {
 getForumListing: PostInquiry.getForumListing,
 doPostAction: PostRequest.doPostAction,
 doLoginAction: UserRequest.doLoginAction
};
