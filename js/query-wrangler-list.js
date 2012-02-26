jQuery(document).ready(function(){

  // delete confirm
  jQuery('.qw-delete-query, .tablenav #doaction').click(function(){
    var ask = confirm('Are you sure you want to delete?');
    if (ask) {
      return true;
    }
    else{
      return false;
    }
  });
});