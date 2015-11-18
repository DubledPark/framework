package org.samchon.protocol.invoke
{
	import mx.collections.ArrayList;
	
	import org.samchon.library.container.IDictionary;
	import org.samchon.protocol.entity.*;
	
	/**
	 * <p> Standard message of network I/O. </p>
	 *
	 * <p> Invoke is a class used in network I/O in protocol package of Samchon Framework.  </p>
	 *
	 * <p> The Invoke message has an XML structure like the result screen of provided example in below. 
	 * We can enjoy lots of benefits by the normalized and standardized message structure used in
	 * network I/O. </p>
	 *
	 * <p> The greatest advantage is that we can make any type of network system, even how the system 
	 * is enourmously complicated. As network communication message is standardized, we only need to
	 * concentrate on logical relationships between network systems. We can handle each network system 
	 * like a object (class) in OOD. And those relationships can be easily designed by using design
	 * pattern. </p>
	 *
	 * <p> In Samchon Framework, you can make any type of network system with basic 3 + 1 componenets
	 * (IProtocol, IServer and IClient + ServerConnector), by implemens or inherits them, like designing
	 * classes of S/W architecture. </p>
	 *
	 * <img src="invoke.png" />
	 *
	 * @author Jeongho Nam
	 */
	public class Invoke
		extends ArrayList//ArrayList<InvokeParameter>
		implements IDictionary
	{
		/**
		 * <p> Represent who listens, often be a function name. </p>
		 */ 
		private var listener:String;
		
		/* ---------------------------------------------------------------------
		CONSTRUCTORS
		--------------------------------------------------------------------- */
		/**
		 * <p> Multiple Constructors. </p>
		 * 
		 * <h4> Construct from a lisetenr </h4>
		 * <p><i> <b>listener</b> := Represents who listens the Invoke message. Almost same with Function name. </i></p>
		 *
		 * <hr /> 
		 * 
		 * <h4> Construct from arguments </h4>
		 * <p> Creates Invoke and InvokeParameter(s) at the same time by varadic template method. </p>
		 * 
		 * <p> By the varadic template constructor, you can't specify name of each InvokeParameter, but
		 * specify type and value of each InvokeParameter. If you try to record the Invoke to Database,
		 * the name of InvokeParameter will be <i>NULL</i>.</p>
		 * 
		 * <p> By the varadic template constructor, name of InovkeParameter(s) will be omitted. Because
		 * of name, an identifier of an InvokeParameter, is omitted, you can't access to InvokeParameter
		 * by Invoke::has() or Invoke::get(). </p>
		 * 
		 * <p><i> <b>listener</b> := Represents who listens the Invoke message. Almost same with Function name. </i></p>
		 * <p><i> <b>arguments</b> := Arguments to be parameters of Invoke. </i></p>
		 * 
		 * <hr />
		 * 
		 * <h4> Construct from an XML object </h4>
		 * 
		 * <p> Constructs Invoke and InvokeParameter objects by an XML object. </p>
		 * <p><i> <b>xml</b> := An xml object representing Invoke object. </i></p>
		 */ 
		public function Invoke(... args)
		{
			super();
			
			if (args.length == 0)
				return;
			else if (args.length == 1)
				if(args[0] is String)
					this.listener = args[0];
				else
					construct(args[1]);
			else
			{
				this.listener = args[0];
				
				for (var i:int = 1; i < args.length; i++)
					addItem(new InvokeParameter("", args[i]));
			}
		}
		
		/**
		 * @copy IEntity.construct()
		 */ 
		public function construct(xml:XML):void
		{
			this.listener = xml.@listener;
			if(xml.hasOwnProperty("parameter") == false)
				return;
			
			var xmlList:XMLList = xml.parameter;
			for(var i:int = 0; i < xmlList.length(); i++)
				addItem( new InvokeParameter(xmlList[i] as XML) );
		}
		
		/* ---------------------------------------------------------------------
			ACCESSORS
		--------------------------------------------------------------------- */
		/**
		 * <p> Get listener. </p>
		 */
		public function getListener():String
		{
			return listener;
		}
		
		/**
		 * <p> Set listener. </p>
		 */
		public function setListener(val:String):void
		{
			this.listener = val;
		}
		
		//ELEMENT ACCESSORS
		/**
		 * <p> Get the <code>InvokeParameter</code> at specified index. </p>
		 * 
		 * @param index the index in the <code>Invoke</code> from which to retrieve the <code>InvokeParameter</code>
		 * @return the <code>InvokeParameter</code> at that index
		 * @throws RangeError if the index &lt; 0 or index &gt;= length
		 */
		public function at(index:int):InvokeParameter
		{
			return getItemAt(index) as InvokeParameter;
		}
		
		/**
		 * <p> Access the <code>InvokeParameter</code> by specified identifier(parameter's name). </p>
		 * 
		 * @param key the identifier of the <code>InvokeParameter</code> wants to access 
		 * @return The <code>InvokeParameter</code> having the key, or null if there is none.
		 */
		public function get(key:*):InvokeParameter
		{
			for(var i:int = 0; i < length; i++)
				if(at(i).getName() == key)
					return at(i);
			
			return null;
		}
		
		//CHECKERS
		public function has(key:*):Boolean
		{
			for(var i:int = 0; i < length; i++)
				if(at(i).getName() == key)
					return true;
			
			return false;
		}
		
		//MODIFIERS
		public function erase(key:*):Boolean
		{
			for(var i:int = length - 1; i >= 0; i--)
				if(at(i).getName() == key)
				{
					removeItemAt(i);
					return true;
				}
			return false;
		}
		
		/**
		 * Get arguments.
		 */
		public function getArguments():Array
		{
			var args:Array = [];
			
			for (var i:int = 0; i < this.length; i++)
				args.push(at(i).getValue());
			
			return args;
		}
		
		/* ---------------------------------------------------------------------
			EXPORTS
		--------------------------------------------------------------------- */
		/**
		 * @copy IEntity.toXML()
		 */
		public function toXML():XML
		{
			var xml:XML = new XML("<invoke />");
			xml.@listener = listener;
			
			var xmlList:XMLList = new XMLList();
			for(var i:int = 0; i < length; i++)
				xmlList[i] = at(i).toXML();
			
			xml.setChildren(xmlList);
			return xml;
		}
	}
}