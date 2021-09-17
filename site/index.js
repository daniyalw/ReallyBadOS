var copyright_id = "copyright";
var refresh = 1000;

function showYear() {
  var x = new Date();
  document.getElementById(copyright_id).innerHTML = x.getFullYear();
  refreshYear();
}

function refreshYear() {
  setTimeout(showYear, refresh);
}
