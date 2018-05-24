var express = require('express');
var OpenForumAPI = require('../OpenForumAPI/Base.js');
var router = express.Router();

router.post('/Post',function(req,res,next) {
 const sessionId=req.cookies.sessionId;
 if (sessionId===undefined)
  throw 'Not logged in';

 const topicSerial=req.body.TopicSerial;
 const subject=req.body.Subject;
 const content=req.body.Content;

 if (topicSerial===undefined)
  throw 'TopicSerial not found';
 if (subject===undefined)
  throw 'Subject not found';
 if (content===undefined)
  throw 'Content not found';

 var arguments={
  LoginToken: sessionId,
  TopicSerial: topicSerial,
  Subject: subject,
  Content: content
 };
 OpenForumAPI.doPostAction('UserPost',arguments,(data)=>{
  res.redirect('/Topic/' + data.TopicSerial);
 });
});

router.get('/register',function(req,res,next) {
 if (req.cookies.sessionId!==undefined) {
  res.redirect('/');
  return;
 }
 res.render('register',{includeLoginStylesheet: true});
});

router.post('/register',function(req,res,next) {
 var arguments={
  Username: req.body.Username,
  Password: req.body.Password
 };
 try
 {
  OpenForumAPI.doLoginAction('Register',arguments,(data)=>{
   res.cookie('sessionId',data);
   res.redirect('/');
  });
 }
 catch(e)
 {res.redirect('/login'); }
});

router.get('/login',function(req,res,next) {
 var args={
  includeLoginStylesheet: true,
  loggedIn: req.cookies.sessionId!==undefined
 };
 res.render('login', args);
});

router.post('/login',function(req,res,next) {
 var arguments={
  Username: req.body.Username,
  Password: req.body.Password
 };
 OpenForumAPI.doLoginAction('LoginVerify',arguments,(data)=>{
  res.cookie('sessionId',data);
  res.redirect('/');
 });
});

router.get('/logout',function(req,res,next) {
 const sessionId=req.cookies.sessionId;
 if (sessionId===undefined) {
  res.redirect('/');
  return next();
 }
 res.clearCookie('sessionId');

 var arguments={
  LoginToken: sessionId
 };
 OpenForumAPI.doLoginAction('Logout',arguments,(data)=>{
  res.redirect('/');
 });
});

router.get('/topic/:topicId',function(req,res,next) {
 OpenForumAPI.getForumListing('TopicInquiry',req.params.topicId,(data)=>{
  var args={
   includeTopicStylesheet:true,
   topic:data
  };
  if (req.cookies.sessionId!==undefined)
   args.loggedIn=true;
  res.render('topic', args);
 });
});

router.get('/board/:boardId',function(req,res,next) {
 OpenForumAPI.getForumListing('BoardInquiry',req.params.boardId,(data) => {
  var args={
   loggedIn: req.cookies.sessionId!==undefined,
   board: data
  };
  res.render('board', args);
 });
});

/* GET boards page. */
router.get('/boards', function(req, res, next) {
 OpenForumAPI.getForumListing('ForumInquiry',null,(data) => {
  var args={
   loggedIn: req.cookies.sessionId!==undefined,
   board: data
  };
  res.render('index', { board: data });
 });
});

router.get('/', function(req, res, next) {
 OpenForumAPI.getForumListing('ForumInquiry',null,(data) => {
  var args={
   loggedIn: req.cookies.sessionId!==undefined,
   board: data
  };
  res.render('index', args);
 });
});

module.exports = router;
