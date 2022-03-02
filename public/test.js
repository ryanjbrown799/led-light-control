'use strict';

const e = React.createElement;

class Button extends React.Component {
  constructor(props) {
    super(props);
    this.state = { click: false };
  }

  render() {
    if (this.state.click) {
      return 'You clicked.';

    }

    return e(
      'button',
      { onClick: () => sub()},
      'clicked'
    );
  }
}

ReactDOM.render(e(Button), document.querySelector('#container'));
//testing 
function sub(){
	var data = document.getElementById("col").value
	var request = new XMLHttpRequest();
	request.open('POST', 'http://localhost:3000/change', true);
	request.setRequestHeader('Content-Type', 'application/x-www-form-urlencoded');
	request.send(data);
}



